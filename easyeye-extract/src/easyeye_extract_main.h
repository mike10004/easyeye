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

namespace easyeye
{
 
class Extract
{
public:
    static const int kErrorIO = ProgramCode::kErrorIO;
    static const int kErrorUsage = ProgramCode::kErrorUsage;
    static const int kErrorOther = ProgramCode::kErrorOther;
    static const char* kProgName;
    
    class ExtractOptions 
    {
    public:
        bool help;
        bool version;
        bool verbose;
        std::string diagnostics_dir;
        ExtractOptions() : help(false), version(false), verbose(false), diagnostics_dir() {}
    };
    
    static void PrintUsage(std::ostream& out);
    static int Main(const int argc, char* argv[]);
	static int Main(const std::vector<std::string>& args);
    static int ParseArgs(const std::vector<std::string>& args, ExtractOptions& options, std::vector<std::string>& positionals);
    static int Main(ExtractOptions& options, const std::vector<std::string>& positionals);    
    static void PrintHelp(std::ostream& out);
    static void PrintVersion(std::ostream& out);
    
};
    
} // end namespace Ezi

#endif	/* EZIEXTRACT_MAIN_H */

