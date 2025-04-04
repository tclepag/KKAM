#pragma once

#include <string>
#include <windows.h>

namespace utils {
    class String {
    public:
        // Constructors
        String() = default;
        String(const char* str) : m_string(str) {}
        String(const wchar_t* wstr) : m_wstring(wstr) {}
        String(const std::string& str) : m_string(str) {}
        String(const std::wstring& wstr) : m_wstring(wstr) {}
        String(const String& other) : m_string(other.m_string), m_wstring(other.m_wstring) {}
        String(String&& other) noexcept : m_string(std::move(other.m_string)), m_wstring(std::move(other.m_wstring)) {}
        String(const char* begin, const char* end) : m_string(begin, end) {}
        String(const wchar_t* begin, const wchar_t* end) : m_wstring(begin, end) {}

        // Conversion operators
        operator std::string() const {
            return m_string.empty() ? wToString(m_wstring) : m_string;
        }

        operator std::wstring() const {
            return m_wstring.empty() ? stringToW(m_string) : m_wstring;
        }

        // Assignment operators
        String& operator=(const std::string& str) {
            m_string = str;
            m_wstring.clear();
            return *this;
        }

        String& operator=(const std::wstring& wstr) {
            m_wstring = wstr;
            m_string.clear();
            return *this;
        }

        String& operator=(const String& other) {
            if (this != &other) {
                m_string = other.m_string;
                m_wstring = other.m_wstring;
            }
            return *this;
        }

        String& operator=(String&& other) noexcept {
            if (this != &other) {
                m_string = std::move(other.m_string);
                m_wstring = std::move(other.m_wstring);
            }
            return *this;
        }

        // Concatenation operators
        String& operator+=(const std::string& str) {
            m_string += str;
            return *this;
        }

        String& operator+=(const std::wstring& wstr) {
            m_wstring += wstr;
            return *this;
        }

        friend String operator+(String lhs, const std::string& rhs) {
            lhs += rhs;
            return lhs;
        }

        friend String operator+(String lhs, const std::wstring& rhs) {
            lhs += rhs;
            return lhs;
        }

        // Comparison operators
        bool operator==(const String& other) const {
            return m_string == other.m_string && m_wstring == other.m_wstring;
        }

        bool operator!=(const String& other) const {
            return !(*this == other);
        }

        String substr(size_t pos, size_t len = std::string::npos) const {
            if (!m_string.empty()) {
                return String(m_string.substr(pos, len));
            }
            else {
                return String(m_wstring.substr(pos, len));
            }
        }

        size_t find_last_of(const String& str, size_t pos = std::string::npos) const {
            if (!m_string.empty()) {
                return m_string.find_last_of(str.m_string, pos);
            }
            else {
                return m_wstring.find_last_of(str.m_wstring, pos);
            }
        }

        std::string toString() const {
            return m_string.empty() ? wToString(m_wstring) : m_string;
        }

        std::wstring toWString() const {
            return m_wstring.empty() ? stringToW(m_string) : m_wstring;
        }

        size_t length() const {
            return m_string.empty() ? m_wstring.length() : m_string.length();
        }

        const char* c_str() const {
            return m_string.c_str();
        }

        const wchar_t* wc_str() const {
            return m_wstring.c_str();
        }

    private:
        std::string m_string;
        std::wstring m_wstring;

        static std::string wToString(const std::wstring& wstr) {
            if (wstr.empty()) return std::string();
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
            std::string strTo(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
            return strTo;
        }

        static std::wstring stringToW(const std::string& str) {
            if (str.empty()) return std::wstring();
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
            return wstrTo;
        }
    };

}