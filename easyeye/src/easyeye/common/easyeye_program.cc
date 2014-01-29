
#include <vector>
#include <string>
#include <iostream>
#include "easyeye_program.h"
#include "easyeye_utils.h"
#include <getopt.h>
#include "mylog.h"

using namespace easyeye::program;
using namespace std;
using easyeye::Vectors;

Options::Options() 
    : action(ERR_USAGE), verbose(false)
{    
}

OptionSpec::OptionSpec(const std::string& long_form_, const char short_form_)
    : long_form(long_form_), short_form(short_form_), arg_spec(NONE)
{
}

OptionSpec::OptionSpec(const std::string& long_form_, const char short_form_, ArgSpec arg_spec_) 
    : long_form(long_form_), short_form(short_form_), arg_spec(arg_spec_)
{
}

OptionSpec::OptionSpec(const std::string& long_form_, ArgSpec arg_spec_) 
    : long_form(long_form_), short_form(0), arg_spec(arg_spec_)
{
}

OptionSpec::OptionSpec(const std::string& long_form_)
    : long_form(long_form_), short_form(0), arg_spec(NONE)
{
}


const char* Program::DescribeCode(int exitCode)
{
    switch(exitCode)
    {
        case kErrorIO: return "kErrorIO";
        case kErrorOther: return "kErrorOther";
        case kErrorUsage: return "kErrorUsage";
        case kExitSuccess: return "Success";
        default: return "Unrecognized";
    }
            
}


Program::Program(const string& name_, const string& version_)
    : options_(), name(name_), version(version_)
{
    AddOption("help", 'h');
    AddOption("version", 'V');
    AddOption("verbose", 'v');
}


Program::~Program() {}


int Program::Main(const int argc, char** argv)
{
    vector<string> args;
    Vectors::AddAll(argv, argc, args);
    return Main(args);
}


void Program::ParseArgs(const std::vector<std::string>& args, std::vector<std::string>& positionals) 
{
    options_.action = CONTINUE;
    int c;
    const int argc = args.size();
    char** argv = new char*[argc];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = (char*) args[i].c_str();
    }
    optind = 0;
    typedef struct option GetOptOption;
    GetOptOption *long_options = new GetOptOption[option_specs_.size() + 1];
    string optstr;
    for (size_t i = 0; i < option_specs_.size(); i++) {
        OptionSpec& spec = option_specs_[i];
        if (spec.short_form != 0) {
            optstr.append(1, spec.short_form);
            if (spec.arg_spec == REQUIRED) {
                optstr.append(1, ':');
            }
        }
        //malloc(sizeof(char*) + sizeof(int) + sizeof(int*) + sizeof(char))
        long_options[i].has_arg = spec.arg_spec;
        long_options[i].name = spec.long_form.c_str();
        long_options[i].flag = 0;
        long_options[i].val = spec.short_form;
    }
    GetOptOption options_end = {0, 0, 0, 0};
    long_options[option_specs_.size()] = options_end;
    while (true && options_.action == CONTINUE) {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        int getopt_optind = optind;
        c = getopt_long (argc, argv, optstr.c_str(), long_options, &option_index);
        if (c == -1) break; /* Detect the end of the options. */
        string option_arg;
        bool has_option_arg = false;
        string long_form;
        if (c == 0 || isalnum(c)) {
            has_option_arg = optarg != NULL;
            if (has_option_arg) {
                option_arg.assign(optarg);
            }
            long_form.assign(long_options[option_index].name);
            if (Eq("help", long_form)) {
                options_.action = HELP;
            } else if (Eq("verbose", long_form)) {
                options_.verbose = true;
            } else if (Eq("version", long_form)) {
                options_.action = VERSION;
            } else {
                OptionParsed(long_form, has_option_arg, option_arg);
            }
        } else {
            options_.action = ERR_USAGE;
        }
    }
    while (optind < argc) {
        positionals.push_back(argv[optind++]);
    }
    delete argv;
    delete long_options;
    
    if (options_.action == CONTINUE) {
        bool positionals_ok = IsPositionalsOk(positionals);
        if (!positionals_ok) {
            options_.action = ERR_USAGE;
        }
    }
    Action action = options().action;
    mylog::Logs::GetLogger().Log(mylog::TRACE, "Program::ParseArgs action = %d\n", action);
}


bool Program::IsPositionalsOk(const vector<std::string>& positionals)
{
    return true;
}


Code Program::Main(const vector<string>& args)
{
    vector<string> positionals;
    ParseArgs(args, positionals);
    Code exit_code;
    switch (options().action) {
        case CONTINUE:
            exit_code = Execute(positionals);
            break;
        case ERR_USAGE:
            PrintUsage(cerr);
            exit_code = kErrorUsage;
            break;
        case HELP:
            PrintHelp(cout);
            exit_code = kExitSuccess;
            break;
        case VERSION:
            PrintVersion(cout);
            exit_code = kExitSuccess;
            break;
        default:
            cerr << name << ": bug; unrecognized action " << options().action << endl;
            exit_code = kErrorOther;
    }
    return exit_code;
}


bool Program::Eq(const std::string& reference, const std::string& query)
{
    return reference.compare(query) == 0;
}


void Program::AddOption(OptionSpec& option_spec)
{
    option_specs_.push_back(option_spec);
}

void Program::AddOption(const std::string& long_form_, const char short_form_)
{
    OptionSpec spec(long_form_, short_form_);
    AddOption(spec);
}

void Program::AddOption(const std::string& long_form_, const char short_form_, ArgSpec arg_spec_)
{
    OptionSpec spec(long_form_, short_form_, arg_spec_);
    AddOption(spec);
}


void Program::AddOption(const std::string& long_form_, ArgSpec arg_spec_)
{
    OptionSpec spec(long_form_, arg_spec_);
    AddOption(spec);
}


void Program::AddOption(const std::string& long_form_)
{
    OptionSpec spec(long_form_);
    AddOption(spec);
}


void Program::OptionParsed(const std::string& long_form, bool arg_present, const std::string& arg)
{
    // do nothing; this is not pure virtual because it may be the case that
    // the program has no options besides --help, --version, and --verbose 
}


const Options& Program::options()
{
    return options_;
}


void Program::PrintHelp(std::ostream& out)
{
    PrintHelpHeader(out);
    PrintUsage(out);
    PrintHelpFooter(out);
    return;
}


void Program::PrintHelpFooter(std::ostream& out)
{
    
}


void Program::PrintHelpHeader(std::ostream& out)
{
    
}


void Program::PrintVersion(std::ostream& out)
{
    out << name << ' ' << version << endl;
}


void Program::PrintUsage(std::ostream& out)
{
}


const char* Program::name_str() 
{
    return name.c_str();
}
