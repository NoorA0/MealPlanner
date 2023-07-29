#include "mainwindow.h"
#include "error_loaddatafailed.h"
#include "mealmanager.h"
#include <fstream>
#include <chrono>
#include <ctime>

#include <QApplication>

int main(int argc, char *argv[])
{
    const QString DATA_NAME = "MealPlanner_Data.txt";
    const QString LOGFILE = "MealPlanner_Logs.txt";
    const double MINIMUM_PRICE = 0.0;    // lowest price for a single meal
    const double MAXIMUM_PRICE = 999.99; // highest price for a single meal
    const unsigned int NAME_LENGTH = 40; // limit for tag and meal names
    const unsigned int DESC_LENGTH = 80; // limit for tag descriptions

    // to R/W data
    std::ifstream iFile;
    std::ofstream oFile;

    MealManager mealManager(MINIMUM_PRICE,
                            MAXIMUM_PRICE,
                            NAME_LENGTH,
                            DESC_LENGTH);
    QApplication a(argc, argv);

    // attempt to load existing data
    try
    {
        if (mealManager.loadState(DATA_NAME, iFile) == 1) // corrupted data
        {
            // write error to log
            std::ofstream errOut(LOGFILE.toStdString(), std::ios::app);

            if (errOut.is_open())
            {
                // get system time
                auto now = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

                errOut << ctime(&currentTime);
                errOut << "ERROR: Saved data is corrupted.\n"
                       << "At least one attritube of a Meal/Tag/MultiTag is invalid in the data file.\n"
                       << "Corrupted values have been set to their defaults. Please check your Meals/Tags/MultiTags for any errors.\n\n";
            }
            errOut.close();

            // display error to user
            Error_LoadDataFailed *err = new Error_LoadDataFailed(nullptr,
                                                                 false,
                                                                 QString());
            err->setAttribute(Qt::WA_DeleteOnClose);
            err->exec();
        }
    }
    catch (QString& error)
    {
        // write error to log
        std::ofstream errOut(LOGFILE.toStdString(), std::ios::app);

        if (errOut.is_open())
        {
            // get system time
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            errOut << ctime(&currentTime);
            errOut << "ERROR: Could not load program data.\n"
                   << "REASON: " << error.toStdString() << "\n"
                   << "Ignore this message if this is the program's first execution.\n\n";

            // display error to user
            Error_LoadDataFailed *err = new Error_LoadDataFailed(nullptr,
                                                                 true,
                                                                 error);
            err->setAttribute(Qt::WA_DeleteOnClose);
            err->exec();
        }
        errOut.close();
    }

    // set seed for RNG
    srand(time(NULL));

    // display main menu
    MainWindow w;
    w.show();
    // TODO: implement data save after w closes, may need to look for signals
    return a.exec();
}
