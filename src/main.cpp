#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "../inc/util.hpp"
#include "../inc/pifunc.hpp"

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;
using std::vector;

const string CURRENTDIR(".");
const string RUNDATA_EXT(".rundata");
const string BATCHDATA_EXT(".batchdata");
const string BATCHDATA_DIR_SUFFIX("_batchdata");

struct bbprun
{
    string name;
    long batchsize;
    long lastdigit;
};

inline bool operator==(const bbprun& left, const bbprun& right)
{
    return left.name == right.name
        && left.batchsize == right.batchsize
        && left.lastdigit == right.lastdigit;
}

const bbprun bbprun_default = { "", 0, 0 };

bbprun prompt_for_saved_run();
bbprun prompt_for_new_run();

int main()
{
    cout <<
        "Welcome to cppbpp!\n"
        "This program uses the Bailey-Borwein-Plouffe algorithm to calculate\n"
        "an infinite amount of hexadecimal digits of pi." << endl;

    cout << endl;

    bbprun currentrun = prompt_for_saved_run();

    cout << endl;
    
    if (currentrun == bbprun_default)
    {
         currentrun = prompt_for_new_run();
    }
    else if (currentrun.batchsize == 0)
    {
        cout << "Batch size is 0, enter a new batch size: ";
        long batchsize;
        while (!(cin >> batchsize) || batchsize < 1)
            cout << "Invalid input, try again: ";
    }

    cout << endl;

    bool paused = true;
    bool stopped = false;

    while (!stopped)
    {
        cout
            << "1 = " << (paused ? "start" : "pause") << "\n"
            << "2 = stop" << endl;

        cout << endl;

        cout << "> ";
        int usernum;
        if (!(cin >> usernum) || usernum < 1 || usernum > 2)
        {
            cout << "Invalid input, try again." << endl;
            continue;
        }
        
        if (usernum == 1)
        {

            paused = !paused;
            
        }
        else
        {
            stopped = true;
        }
    }

    return 0;
}

bbprun load_run(string& run_name)
{
    long batchsize, lastdigit;
    bbprun ret;
    ifstream savedrun(run_name + RUNDATA_EXT);
    if (savedrun && savedrun >> batchsize >> lastdigit)
    {
        cout << "Loaded run " << run_name << "." << endl;
        ret = { run_name, batchsize, lastdigit };
    }
    else
    {
        cout << "Unable to load run " << run_name << "." << endl;
        ret = bbprun_default;
    }
    savedrun.close();
    return ret;
}

bbprun prompt_for_saved_run()
{
    string userin;
    vector<string> savedrun_names;
    string selectedrun_name;

    // get list of saved runs from files with .rundata extension
    for (fs::path p : fs::directory_iterator(CURRENTDIR))
    {
        if (p.extension() == RUNDATA_EXT)
            savedrun_names.push_back(p.stem().string());
    }

    // if 1 saved run, ask user if they'd like to continue that run
    if (savedrun_names.size() == 1)
    {
        cout << "1 previous run detected, continue with that run? y/n: ";
        while (!(cin >> userin) || (userin != "y" && userin != "n"))
            cout << "Invalid input, try again. y/n: ";
        if (userin == "n")
            return bbprun_default;

        selectedrun_name = savedrun_names[0];
    }
    // if more than 1 saved run, ask user if they'd like to choose a run
    else if (savedrun_names.size() > 1)
    {
        cout << savedrun_names.size() << " previous runs detected, "
            "select one to continue? y/n: ";
        while (!(cin >> userin) || (userin != "y" && userin != "n"))
            cout << "Invalid input, try again. y/n: ";
        if (userin == "n")
            return bbprun_default;
        
        cout << "List of saved runs: " << endl;
        for (size_t i = 1; i <= savedrun_names.size(); ++i)
        {
            cout << i << ": " << savedrun_names[i - 1] << "\n";
        }
        cout << "Enter the number for the save you want to load: ";
        long usernum;
        while (!(cin >> usernum) || usernum < 1 || usernum > savedrun_names.size())
            cout << "Invalid input, try again: ";

        selectedrun_name = savedrun_names[usernum - 1];
    }
    // if no saved runs, return
    else
    {
        return bbprun_default;
    }

    return load_run(selectedrun_name);
}

bbprun prompt_for_new_run()
{
    cout << "Starting a new run." << endl;
    cout <<
        "Batch size determines how many digits the program will\n"
        "calculate at once using multithreading. It is recommended\n"
        "to use a batch size of 1000 * processor cores.\n";
    cout << "Enter a batch size (greater than 0): ";
    long batchsize;
    while (!(cin >> batchsize) || batchsize < 1)
        cout << "Invalid input, try again: ";

    return { "", batchsize, 0 };
}