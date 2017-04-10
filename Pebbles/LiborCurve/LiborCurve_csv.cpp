//
// Created by CUI Shidong on 6/4/2017.
//

#include <ql/settings.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
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

    // Abstract data from a CSV file

    std::ifstream dataFile;
    dataFile.open("LIBOR_OIS.csv", std::ios::in);
    bool a = dataFile.is_open();
    std::string line;
    dataFile >> line;
    dataFile >> line;
    std::getline(dataFile, line, '\n');
    std::string line2;
    std::getline(dataFile, line2);
    std::getline(dataFile, line2);
    std::getline(dataFile, line2);
    std::getline(dataFile, line2);
    dataFile.close();

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