#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <fstream>
#include <string>


class Logger
{

    public:
        Logger()
        { }

        ~Logger()
        { }

        void logtime(char x = 'o')
        {
            logfile_record(x);
        }

        void logfile_record(char state)
        {
            get_time_now();
            //Create or Append File
            std::ofstream file;
            if (state == 'c')
            {
                file.open("../logfile.txt", std::ios::app);
                file << "Closed: " << curr_time + "\n" << std::endl; 
            }
            else if (state == 'o')
            {
                file.open("../logfile.txt", std::ios::app);
                file << "Opened at: " << curr_time << std::endl; 
            }

        }

        void get_time_now()
        {
            time_t now = time(0);
            char *dt = ctime(&now);
            curr_time = dt;
        }

        void log_event(std::string status)
        {
            std::ofstream file;
            file.open("../logfile.txt", std::ios::app);
            if(file.is_open())
            {
                file << "Event: " << status << std::endl;
            }
        }

        

    private:
        std::string curr_time;
};


#endif