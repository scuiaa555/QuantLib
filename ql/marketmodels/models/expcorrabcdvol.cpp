/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006, 2007 Ferdinando Ametrano
 Copyright (C) 2006 Mark Joshi
 Copyright (C) 2005, 2006 Klaus Spanderen

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/marketmodels/models/expcorrabcdvol.hpp>
#include <ql/termstructures/volatilities/abcd.hpp>
#include <ql/math/pseudosqrt.hpp>

namespace QuantLib {

    ExpCorrAbcdVol::ExpCorrAbcdVol(
            Real a,
            Real b,
            Real c,
            Real d,
            const std::vector<Real>& ks,
            const Matrix& correlations,
            const EvolutionDescription& evolution,
            const Size numberOfFactors,
            const std::vector<Rate>& initialRates,
            const std::vector<Spread>& displacements)
    : numberOfFactors_(numberOfFactors),
      numberOfRates_(initialRates.size()),
      numberOfSteps_(evolution.evolutionTimes().size()),
      initialRates_(initialRates),
      displacements_(displacements),
      evolution_(evolution),
      pseudoRoots_(numberOfSteps_, Matrix(numberOfRates_, numberOfFactors_)),
      covariance_(numberOfSteps_, Matrix(numberOfRates_, numberOfRates_)),
      totalCovariance_(numberOfSteps_, Matrix(numberOfRates_, numberOfRates_))
    {
        const std::vector<Time>& rateTimes = evolution.rateTimes();
        QL_REQUIRE(numberOfRates_==rateTimes.size()-1,
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and rate times");
        QL_REQUIRE(numberOfRates_==displacements.size(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and displacements (" << displacements.size() << ")");
        QL_REQUIRE(numberOfRates_==ks.size(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and ks (" << ks.size() << ")");
        QL_REQUIRE(numberOfRates_<=numberOfFactors_*numberOfSteps_,
                   "number of rates (" << numberOfRates_ <<
                   ") greater than number of factors (" << numberOfFactors_ 
                   << ") times number of steps (" << numberOfSteps_ << ")");
        QL_REQUIRE(numberOfRates_==correlations.rows(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and correlation rows (" << correlations.rows() << ")");
        QL_REQUIRE(numberOfRates_==correlations.columns(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and correlation columns (" << correlations.columns() << ")");

        Time effStartTime;
        Real covar;
        Abcd abcd(a, b, c, d);
        const Matrix& effectiveStopTime = evolution.effectiveStopTime();
        for (Size l=0; l<numberOfRates_; ++l) {
            for (Size i=0; i<numberOfRates_; ++i) {
                for (Size j=i; j<numberOfRates_; ++j) {
                    effStartTime = l>0 ? effectiveStopTime[l-1][i] : 0.0;
                    covar = abcd.covariance(effStartTime,
                                            effectiveStopTime[l][i],
                                            rateTimes[i], rateTimes[j]);
                    Real correlation;
                    if (i>=l && j >=l)
                        correlation = correlations[i-l][j-l];
                    else
                        correlation = 0;
                    covariance_[l][j][i] = covariance_[l][i][j] = 
                            ks[i] * ks[j] * covar * correlation;
                 }
             }

            pseudoRoots_[l] =
                rankReducedSqrt(covariance_[l], numberOfFactors, 1.0,
                                 SalvagingAlgorithm::None);

            totalCovariance_[l] = covariance_[l];
            if (l>0)
                totalCovariance_[l] += totalCovariance_[l-1];

            QL_ENSURE(pseudoRoots_[l].rows()==numberOfRates_,
                      "step " << l
                      << " abcd vol wrong number of rows: "
                      << pseudoRoots_[l].rows()
                      << " instead of " << numberOfRates_);
            QL_ENSURE(pseudoRoots_[l].columns()==numberOfFactors,
                      "step " << l
                      << " abcd vol wrong number of columns: "
                      << pseudoRoots_[l].columns()
                      << " instead of " << numberOfFactors);
        }
    }

}