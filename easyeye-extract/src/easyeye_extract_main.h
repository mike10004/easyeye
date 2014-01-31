/* 
 * File:   easyeye_extract_main.h
 * Author: mchaberski
 *
 * Created on July 18, 2013, 12:55 PM
 */

#ifndef EASYEYE_EXTRACT_MAIN_H
#define	EASYEYE_EXTRACT_MAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <easyeye/common/easyeye_types.h>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye/segment/easyeye_segment.h>
#include <easyeye/common/easyeye_program.h>

namespace easyeye
{
namespace extract
{

class ExtractOptions : public program::Options
{
public:
    ExtractOptions();
    std::string diagnostics_dir;
    std::string csv_file;
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
    void OptionParsed(const std::string& long_form, bool arg_present, const std::string& arg);
private:
    ExtractOptions extract_options_;
    void PrintCsvResult(const std::string& image_pathname, const easyeye::Segmentation& seg);
};
    }    // end namespace extract
} // end namespace easyeye

#endif	/* EASYEYE_EXTRACT_MAIN_H */

