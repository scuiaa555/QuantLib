//
// Created by CUI Shidong on 5/4/2017.
//

#include <ql/auto_link.hpp>
#include <ql/settings.hpp>
#include <ql/time/all.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/termstructures/yield/oisratehelper.hpp>
#include <ql/indexes/ibor/eonia.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>


#include <boost/timer.hpp>
#include <iostream>
#include <fstream>

using namespace QuantLib;

int main(int, char *[]) {

    boost::timer timer;
    std::cout << std::endl;

    /*********************
     ***  MARKET DATA  ***
     *********************/

    Calendar calendar = TARGET();
    Date todaysDate(11, December, 2012);
    // must be a business day
    Settings::instance().evaluationDate() = todaysDate;

    /* Pillar instruments */
    // One-day deposit (ON,TN,SN) rates for the short end
    Rate dONQuote = 0.04 / 100;  // Over-Night deposit
    Rate dTNQuote = 0.04 / 100;  // Tomorrow-Next deposit
    Rate dSNQuote = 0.04 / 100;  // Spot-Next deposit

    boost::shared_ptr<Quote> dONRate(new SimpleQuote(dONQuote));
    boost::shared_ptr<Quote> dTNRate(new SimpleQuote(dTNQuote));
    boost::shared_ptr<Quote> dSNRate(new SimpleQuote(dSNQuote));

    boost::shared_ptr<RateHelper> dON(new DepositRateHelper(
            Handle<Quote>(dONRate), Period(1, Days), 0, calendar, Following, false, Actual360()));
    boost::shared_ptr<RateHelper> dTN(new DepositRateHelper(
            Handle<Quote>(dTNRate), Period(1, Days), 1, calendar, Following, false, Actual360()));
    boost::shared_ptr<RateHelper> dSN(new DepositRateHelper(
            Handle<Quote>(dSNRate), Period(1, Days), 2, calendar, Following, false, Actual360()));

    // OIS quotes for the first month
    Rate dSWQuote = 0.07 / 100;
    Rate d2WQuote = 0.069 / 100;
    Rate d3WQuote = 0.078 / 100;
    Rate d1MQuote = 0.074 / 100;

    boost::shared_ptr<Quote> dSWRate(new SimpleQuote(dSWQuote));
    boost::shared_ptr<Quote> d2WRate(new SimpleQuote(d2WQuote));
    boost::shared_ptr<Quote> d3WRate(new SimpleQuote(d3WQuote));
    boost::shared_ptr<Quote> d1MRate(new SimpleQuote(d1MQuote));

    boost::shared_ptr<Eonia> eonia(new Eonia());
    boost::shared_ptr<RateHelper> dSW(new OISRateHelper(2, Period(1, Weeks), Handle<Quote>(dSWRate), eonia));
    boost::shared_ptr<RateHelper> d2W(new OISRateHelper(2, Period(2, Weeks), Handle<Quote>(d2WRate), eonia));
    boost::shared_ptr<RateHelper> d3W(new OISRateHelper(2, Period(3, Weeks), Handle<Quote>(d3WRate), eonia));
    boost::shared_ptr<RateHelper> d1M(new OISRateHelper(2, Period(1, Months), Handle<Quote>(d1MRate), eonia));

    // OIS forwards on ECB dates
    Rate f1Quote = 0.046 / 100;
    Rate f2Quote = 0.016 / 100;
    Rate f3Quote = -0.007 / 100;
    Rate f4Quote = -0.013 / 100;
    Rate f5Quote = -0.014 / 100;

    boost::shared_ptr<Quote> f1Rate(new SimpleQuote(f1Quote));
    boost::shared_ptr<Quote> f2Rate(new SimpleQuote(f2Quote));
    boost::shared_ptr<Quote> f3Rate(new SimpleQuote(f3Quote));
    boost::shared_ptr<Quote> f4Rate(new SimpleQuote(f4Quote));
    boost::shared_ptr<Quote> f5Rate(new SimpleQuote(f5Quote));

    boost::shared_ptr<RateHelper> f1(new DatedOISRateHelper(
            Date(16, January, 2013), Date(13, February, 2013), Handle<Quote>(f1Rate), eonia));
    boost::shared_ptr<RateHelper> f2(new DatedOISRateHelper(
            Date(13, February, 2013), Date(13, March, 2013), Handle<Quote>(f2Rate), eonia));
    boost::shared_ptr<RateHelper> f3(new DatedOISRateHelper(
            Date(13, March, 2013), Date(10, April, 2013), Handle<Quote>(f3Rate), eonia));
    boost::shared_ptr<RateHelper> f4(new DatedOISRateHelper(
            Date(10, April, 2013), Date(8, May, 2013), Handle<Quote>(f4Rate), eonia));
    boost::shared_ptr<RateHelper> f5(new DatedOISRateHelper(
            Date(8, May, 2013), Date(12, June, 2013), Handle<Quote>(f5Rate), eonia));

    // OIS up to 30 years
    Rate s15mQuote = 0.002 / 100;
    Rate s18mQuote = 0.008 / 100;
    Rate s21mQuote = 0.021 / 100;
    Rate s2yQuote = 0.036 / 100;
    Rate s3yQuote = 0.127 / 100;
    Rate s4yQuote = 0.274 / 100;
    Rate s5yQuote = 0.456 / 100;
    Rate s6yQuote = 0.647 / 100;
    Rate s7yQuote = 0.827 / 100;
    Rate s8yQuote = 0.996 / 100;
    Rate s9yQuote = 1.147 / 100;
    Rate s10yQuote = 1.280 / 100;
    Rate s11yQuote = 1.404 / 100;
    Rate s12yQuote = 1.516 / 100;
    Rate s15yQuote = 1.764 / 100;
    Rate s20yQuote = 1.939 / 100;
    Rate s25yQuote = 2.003 / 100;
    Rate s30yQuote = 2.038 / 100;

    boost::shared_ptr<Quote> s15mRate(new SimpleQuote(s15mQuote));
    boost::shared_ptr<Quote> s18mRate(new SimpleQuote(s18mQuote));
    boost::shared_ptr<Quote> s21mRate(new SimpleQuote(s21mQuote));
    boost::shared_ptr<Quote> s2yRate(new SimpleQuote(s2yQuote));
    boost::shared_ptr<Quote> s3yRate(new SimpleQuote(s3yQuote));
    boost::shared_ptr<Quote> s4yRate(new SimpleQuote(s4yQuote));
    boost::shared_ptr<Quote> s5yRate(new SimpleQuote(s5yQuote));
    boost::shared_ptr<Quote> s6yRate(new SimpleQuote(s6yQuote));
    boost::shared_ptr<Quote> s7yRate(new SimpleQuote(s7yQuote));
    boost::shared_ptr<Quote> s8yRate(new SimpleQuote(s8yQuote));
    boost::shared_ptr<Quote> s9yRate(new SimpleQuote(s9yQuote));
    boost::shared_ptr<Quote> s10yRate(new SimpleQuote(s10yQuote));
    boost::shared_ptr<Quote> s11yRate(new SimpleQuote(s11yQuote));
    boost::shared_ptr<Quote> s12yRate(new SimpleQuote(s12yQuote));
    boost::shared_ptr<Quote> s15yRate(new SimpleQuote(s15yQuote));
    boost::shared_ptr<Quote> s20yRate(new SimpleQuote(s20yQuote));
    boost::shared_ptr<Quote> s25yRate(new SimpleQuote(s25yQuote));
    boost::shared_ptr<Quote> s30yRate(new SimpleQuote(s30yQuote));

    boost::shared_ptr<RateHelper> s15m(new OISRateHelper(2, Period(15, Months), Handle<Quote>(s15mRate), eonia));
    boost::shared_ptr<RateHelper> s18m(new OISRateHelper(2, Period(18, Months), Handle<Quote>(s18mRate), eonia));
    boost::shared_ptr<RateHelper> s21m(new OISRateHelper(2, Period(21, Months), Handle<Quote>(s21mRate), eonia));
    boost::shared_ptr<RateHelper> s2y(new OISRateHelper(2, Period(2, Years), Handle<Quote>(s2yRate), eonia));
    boost::shared_ptr<RateHelper> s3y(new OISRateHelper(2, Period(3, Years), Handle<Quote>(s3yRate), eonia));
    boost::shared_ptr<RateHelper> s4y(new OISRateHelper(2, Period(4, Years), Handle<Quote>(s4yRate), eonia));
    boost::shared_ptr<RateHelper> s5y(new OISRateHelper(2, Period(5, Years), Handle<Quote>(s5yRate), eonia));
    boost::shared_ptr<RateHelper> s6y(new OISRateHelper(2, Period(6, Years), Handle<Quote>(s6yRate), eonia));
    boost::shared_ptr<RateHelper> s7y(new OISRateHelper(2, Period(7, Years), Handle<Quote>(s7yRate), eonia));
    boost::shared_ptr<RateHelper> s8y(new OISRateHelper(2, Period(8, Years), Handle<Quote>(s8yRate), eonia));
    boost::shared_ptr<RateHelper> s9y(new OISRateHelper(2, Period(9, Years), Handle<Quote>(s9yRate), eonia));
    boost::shared_ptr<RateHelper> s10y(new OISRateHelper(2, Period(10, Years), Handle<Quote>(s10yRate), eonia));
    boost::shared_ptr<RateHelper> s11y(new OISRateHelper(2, Period(11, Years), Handle<Quote>(s11yRate), eonia));
    boost::shared_ptr<RateHelper> s12y(new OISRateHelper(2, Period(12, Years), Handle<Quote>(s12yRate), eonia));
    boost::shared_ptr<RateHelper> s15y(new OISRateHelper(2, Period(15, Years), Handle<Quote>(s15yRate), eonia));
    boost::shared_ptr<RateHelper> s20y(new OISRateHelper(2, Period(20, Years), Handle<Quote>(s20yRate), eonia));
    boost::shared_ptr<RateHelper> s25y(new OISRateHelper(2, Period(25, Years), Handle<Quote>(s25yRate), eonia));
    boost::shared_ptr<RateHelper> s30y(new OISRateHelper(2, Period(30, Years), Handle<Quote>(s30yRate), eonia));

    std::vector<boost::shared_ptr<RateHelper> > instruments(0);
    instruments.push_back(dON);
    instruments.push_back(dTN);
    instruments.push_back(dSN);
    instruments.push_back(dSW);
    instruments.push_back(d2W);
    instruments.push_back(d3W);
    instruments.push_back(d1M);
    instruments.push_back(f1);
    instruments.push_back(f2);
    instruments.push_back(f3);
    instruments.push_back(f4);
    instruments.push_back(f5);
    instruments.push_back(s15m);
    instruments.push_back(s18m);
    instruments.push_back(s21m);
    instruments.push_back(s2y);
    instruments.push_back(s3y);
    instruments.push_back(s4y);
    instruments.push_back(s5y);
    instruments.push_back(s6y);
    instruments.push_back(s7y);
    instruments.push_back(s8y);
    instruments.push_back(s9y);
    instruments.push_back(s10y);
    instruments.push_back(s11y);
    instruments.push_back(s12y);
    instruments.push_back(s15y);
    instruments.push_back(s20y);
    instruments.push_back(s25y);
    instruments.push_back(s30y);


    /* Yield curve construction */
    boost::shared_ptr<YieldTermStructure> yieldCurve;

    // Monotonic log cubic natural spline
    LogCubic mlcns(CubicInterpolation::Spline);

    yieldCurve = boost::shared_ptr<YieldTermStructure>(
            new PiecewiseYieldCurve<Discount, LogCubic>(0, calendar, instruments, Actual365Fixed(), mlcns));

    yieldCurve->enableExtrapolation();


    /* Plot and save the OIS forward curve */
    Date start = todaysDate;
    Date end = start + Period(2, Years);
    std::vector<Rate> rates_1d(0);
    Date current = start;
    for (int i = 0; i < end.serialNumber() - start.serialNumber(); ++i) {
        Rate tmp = yieldCurve->forwardRate(current, Period(1, Days), Actual360(), Simple).rate();
        rates_1d.push_back(tmp);
        current = Date(current.serialNumber() + 1);
    }

    // Write the results to CSV file
    std::ofstream file;
    file.open("zero-rates.csv");
    for (int i = 0; i < rates_1d.size(); ++i) {
        file << rates_1d[i] << ",";
    }
    file.close();


    double seconds = timer.elapsed();
    Integer hours = int(seconds / 3600);
    seconds -= hours * 3600;
    Integer minutes = int(seconds / 60);
    seconds -= minutes * 60;
    std::cout << " \nRun completed in ";
    if (hours > 0)
        std::cout << hours << " h ";
    if (hours > 0 || minutes > 0)
        std::cout << minutes << " m ";
    std::cout << std::fixed << std::setprecision(0)
              << seconds << " s\n" << std::endl;

    return 0;
}