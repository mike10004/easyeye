
#include <vector>
#include <string>
#include <iostream>
#include "easyeye_program.h"
#include "easyeye_utils.h"
#include <getopt.h>

using namespace easyeye::program;
using namespace std;
using easyeye::Vectors;

Options::Options() 
    : action(ERR_USAGE), verbose(false)
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


const char* Program::Describe(Code exitCode)
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
    int c;
    const int argc = args.size();
    char** argv = new char*[argc];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = (char*) args[i].c_str();
    }
    int version_flag = 0, help_flag = 0, verbose_flag = 0;
    typedef struct option GetOptOption;
    GetOptOption *long_options = new GetOptOption[option_specs_.size() + 1];
    int dummy;
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
        long_options[i].val = spec.short_form == 0 ? i : spec.short_form;
    }
    GetOptOption options_end = {0, 0, 0, 0};
    long_options[option_specs_.size()] = options_end;
    bool option_error = false;
    while (true && !option_error) {
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long (argc, argv, "abc:d:f:", long_options, &option_index);
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
                options().action = HELP;
            } else if (Eq("verbose", long_form)) {
                options().verbose = true;
            } else if (Eq("version", long_form)) {
                options().action = VERSION;
            } else {
                OptionParsed(long_form, has_option_arg, option_arg);
            }
        } else {
            option_error = true;
        }
    }
    while (optind < argc) {
        positionals.push_back(argv[optind++]);
    }
    delete argv;
    delete long_options;
    if (!IsPositionalsOk(positionals)) {
        option_error = true;
    }
    if (option_error) {
        options().action = ERR_USAGE;
    }
}


bool Program::IsPositionalsOk(const vector<std::string>& positionals)
{
    return true;
}


Code Program::Main(const vector<string>& args)
{
    vector<string> positionals;
    ParseArgs(args, positionals);
    switch (options().action) {
        case CONTINUE:
            break;
        case ERR_USAGE:
            PrintUsage(cerr);
            return kErrorUsage;
        case HELP:
            PrintHelp(cout);
            break;
        case VERSION:
            PrintVersion(cout);
            break;
        default:
            assert(false);
    }
    return kExitSuccess;
}


bool Program::Eq(const std::string& reference, const std::string& query)
{
    return reference.compare(query) == 0;
}


void Program::AddOption(OptionSpec& option_spec)
{
    option_specs_.push_back(option_spec);
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


Options& Program::options()
{
    return dynamic_cast<Options&>(options_);
}


void Program::PrintHelp(std::ostream& out)
{
    PrintHelpHeader(out);
    PrintUsage(out);
    PrintHelpFooter(out);
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