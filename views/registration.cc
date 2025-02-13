//this file is generated by program(drogon_ctl) automatically,don't modify it!
#include "registration.h"
#include <drogon/utils/OStringStream.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <deque>
#include <queue>
 #include <drogon/utils/OStringStream.h> 
using namespace drogon;
std::string registration::genText(const DrTemplateData& registration_view_data)
{
	drogon::OStringStream registration_tmp_stream;
	std::string layoutName{""};
	registration_tmp_stream << "<head>\n";
	registration_tmp_stream << "    <meta charset=\"UTF-8\">\n";
	registration_tmp_stream << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	registration_tmp_stream << "    <title>Register</title>\n";
	registration_tmp_stream << "    <link rel=\"stylesheet\" href=\"/css/register.css\">\n";
	registration_tmp_stream << "</head>\n";
	registration_tmp_stream << "<body>\n";
	registration_tmp_stream << "    <div class=\"container\">\n";
registration_tmp_stream<<"\n";
	registration_tmp_stream << "        <span class=\"error-message\">";
{
    auto & val=registration_view_data["error"];
    if(val.type()==typeid(const char *)){
        registration_tmp_stream<<*(std::any_cast<const char *>(&val));
    }else if(val.type()==typeid(std::string)||val.type()==typeid(const std::string)){
        registration_tmp_stream<<*(std::any_cast<const std::string>(&val));
    }
}
	registration_tmp_stream << "</span>\n";
	registration_tmp_stream << "        <form class=\"register-form\" action=\"/register\" method=\"POST\">\n";
registration_tmp_stream<<"\n";
	registration_tmp_stream << "            <input type=\"hidden\" name=\"_csrf\" value=\"";
{
    auto & val=registration_view_data["csrf_token"];
    if(val.type()==typeid(const char *)){
        registration_tmp_stream<<*(std::any_cast<const char *>(&val));
    }else if(val.type()==typeid(std::string)||val.type()==typeid(const std::string)){
        registration_tmp_stream<<*(std::any_cast<const std::string>(&val));
    }
}
	registration_tmp_stream << "\">\n";
registration_tmp_stream<<"\n";
	registration_tmp_stream << "            <h2>";
{
    auto & val=registration_view_data["title"];
    if(val.type()==typeid(const char *)){
        registration_tmp_stream<<*(std::any_cast<const char *>(&val));
    }else if(val.type()==typeid(std::string)||val.type()==typeid(const std::string)){
        registration_tmp_stream<<*(std::any_cast<const std::string>(&val));
    }
}
	registration_tmp_stream << "</h2>\n";
	registration_tmp_stream << "            <div class=\"form-group\">\n";
	registration_tmp_stream << "                <input type=\"text\" name=\"username\" placeholder=\"Username\" required>\n";
	registration_tmp_stream << "            </div>\n";
	registration_tmp_stream << "            <div class=\"form-group\">\n";
	registration_tmp_stream << "                <input id=\"email\" type=\"email\" name=\"email\" placeholder=\"Email\" required>\n";
	registration_tmp_stream << "            </div>\n";
	registration_tmp_stream << "            <div class=\"form-group\">\n";
	registration_tmp_stream << "                <input type=\"password\" name=\"password\" placeholder=\"Password\" required>\n";
	registration_tmp_stream << "            </div>\n";
	registration_tmp_stream << "            <button type=\"submit\">Register</button>\n";
	registration_tmp_stream << "            <p class=\"login-link\">Already have an account? <a href=\"/login\">Login</a></p>\n";
	registration_tmp_stream << "        </form>\n";
	registration_tmp_stream << "    </div>\n";
	registration_tmp_stream << "    <div>\n";
	registration_tmp_stream << "        <h3 id=\"current_date\"></h3>\n";
	registration_tmp_stream << "    </div>\n";
registration_tmp_stream<<"\n";
	registration_tmp_stream << "    <script src=\"/js/authy.js\" defer></script>\n";
	registration_tmp_stream << "</body>\n";
	registration_tmp_stream << "</html>\n";
if(layoutName.empty())
{
std::string ret{std::move(registration_tmp_stream.str())};
return ret;
}else
{
auto templ = DrTemplateBase::newTemplate(layoutName);
if(!templ) return "";
HttpViewData data = registration_view_data;
auto str = std::move(registration_tmp_stream.str());
if(!str.empty() && str[str.length()-1] == '\n') str.resize(str.length()-1);
data[""] = std::move(str);
return templ->genText(data);
}
}
