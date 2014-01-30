/* 
 * File:   easyeye_program.h
 * Author: ibgmike
 *
 * Created on January 29, 2014, 9:31 AM
 */

#ifndef EASYEYE_PROGRAM_H
#define	EASYEYE_PROGRAM_H

#include <string>
#include <vector>
#include <getopt.h>

namespace easyeye
{
    namespace program
    {
    enum ArgSpec { NONE = no_argument, REQUIRED = required_argument};
    enum Action { ERR_USAGE, HELP, VERSION, CONTINUE };
    class OptionSpec
    {
    public:
        std::string long_form;
        char short_form;
        ArgSpec arg_spec;
        std::string help_text;
        std::string metavar;
        OptionSpec(const std::string& long_form_, const char short_form_, ArgSpec arg_spec_);
        OptionSpec(const std::string& long_form_, const char short_form_);
        OptionSpec(const std::string& long_form_, ArgSpec arg_spec_);
        OptionSpec(const std::string& long_form_);
    };
    class Options
    {
    public:
        Options();
        Action action;
        bool verbose;
    };

    enum Code
    {
    kErrorIO = 2, kErrorUsage = 1, kExitSuccess = 0, kErrorOther = 3
    };
    
class Program
{

public:
    Program(const std::string& name, const std::string& version);
    virtual ~Program();
    int Main(const int argc, char** argv);
    Code Main(const std::vector<std::string>& args);
    static bool Eq(const std::string& reference, const std::string& query);
    void AddOption(OptionSpec& option_spec);
    void AddOption(const std::string& long_form_, const char short_form_, ArgSpec arg_spec_);
    void AddOption(const std::string& long_form_, const char short_form_);
    void AddOption(const std::string& long_form_, ArgSpec arg_spec_);
    void AddOption(const std::string& long_form_);
    virtual void OptionParsed(const std::string& long_form, bool arg_present, const std::string& arg);
    virtual bool IsPositionalsOk(const std::vector<std::string>& positionals);
    virtual Code Execute(const std::vector<std::string>& positionals) = 0;
    const Options& options();
    static const char* DescribeCode(int code);
    virtual void PrintHelp(std::ostream& out);
    virtual void PrintUsage(std::ostream& out);
    virtual void PrintVersion(std::ostream& out);
    virtual void PrintHelpHeader(std::ostream& out);
    virtual void PrintHelpFooter(std::ostream& out);
    const char* name_str();
    const OptionSpec& FindSpec(char short_form) const;
protected:
    Options options_;
    const std::string name;
    const std::string version;
private:
    std::vector<OptionSpec> option_specs_;
    void ParseArgs(const std::vector<std::string>& args, std::vector<std::string>& positionals);
    const static OptionSpec null_option_spec_;
};
    } // ns program
} // ns easyeye
#endif	/* EASYEYE_PROGRAM_H */

