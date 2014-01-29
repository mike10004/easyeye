/* 
 * File:   eziextract_main.h
 * Author: mchaberski
 *
 * Created on July 18, 2013, 12:55 PM
 */

#ifndef EZIEXTRACT_MAIN_H
#define	EZIEXTRACT_MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <easyeye/common/easyeye_types.h>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye/common/easyeye_program.h>

namespace easyeye
{
    namespace extract
    {
class ExtractOptions : public program::Options
{
public:
    std::string diagnostics_dir;
    ExtractOptions() : program::Options(), diagnostics_dir() {}
};

    
class Extract : public easyeye::program::Program
{
public:
    Extract();
    ~Extract();
    program::Code Execute(const std::vector<std::string>& positionals);    
    void PrintHelpHeader(std::ostream& out);
    void PrintHelpFooter(std::ostream& out);
    void PrintUsage(std::ostream& out);
    bool IsPositionalsOk(const std::vector<std::string>& positionals);
    void OptionParsed(const std::string& long_form, bool has_arg, const std::string optarg);
private:
    ExtractOptions extract_options_;
};
    }    // end namespace extract
} // end namespace easyeye

#endif	/* EZIEXTRACT_MAIN_H */

