#ifndef _EXPORTHEADER_HPP
#define _EXPORTHEADER_HPP

#ifdef WIN32
    #define ExportSymbol   __declspec(dllexport)
#else
    #define ExportSymbol
#endif

#endif //_EXPORTHEADER_HPP
