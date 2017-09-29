//
//  ScriptTextDelegate.h
//  Engine_Mac
//
//  Created by True on 08/05/15.
//
//

#ifndef __Engine_Mac__ScriptTextDelegate__
#define __Engine_Mac__ScriptTextDelegate__

#include <string>
#include <functional>

class ScriptTextDelegate
{
public:
    struct TextData
    {
        TextData(std::string& name, std::string& value, std::string& attributes, std::string& language);
        
        std::string& name_;
        std::string& value_;
        std::string& attributes_;
        std::string& language_;
    };
    
    typedef std::function<void(TextData& textData)> Delegate;
    static void resetDelegate(const Delegate& delegate);
    
    static void invokeDelegate(std::string& name, std::string& value, std::string& attributes, std::string& language);
    
private:
    class Impl;
};


#endif /* defined(__Engine_Mac__ScriptTextDelegate__) */
