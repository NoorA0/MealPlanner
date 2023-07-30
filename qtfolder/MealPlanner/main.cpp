#include "mainwindow.h"
#include "error_loaddatafailed.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // to R/W data
    std::ifstream iFile;
    std::ofstream oFile;

    MealManager mealManager;
    QApplication a(argc, argv);

    // attempt to load existing data
    try
    {
        if (mealManager.loadState(iFile) == 1) // corrupted data
        {
            // write error to log
            std::ofstream errOut(mealManager.getLogFileName().toStdString(), std::ios::app);

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
        std::ofstream errOut(mealManager.getLogFileName().toStdString(), std::ios::app);

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
    MainWindow w(nullptr,
                 &mealManager);
    w.show();
    return a.exec();
}
