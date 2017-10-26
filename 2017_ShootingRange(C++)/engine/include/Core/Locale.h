#ifndef _CORE_LOCALE_H_
#define _CORE_LOCALE_H_

namespace Core
{

class Locale
{
public:

	Locale();

	//
	// Игнорирование пробелов, для японского.
	// Не печатается пробел 0x20, пробел 0xA0 - печатается.
	//
	void SetIgnoreSpaces(bool ignore);
	bool GetIgnoreSpaces() const;

	// Включить обработку нескольких языков.
	// Если forceLang пустой, то используется системный язык, иначе заданный (de, fr, it и т.д.).
	// По умолчанию MUI не включено.
	void UseMui(const std::string &forceLang = "");

	bool IsMuiUsed() const;

	// Вернуть идентификатор языка (en, de, fr и т.д.).
	// По умолчанию en.
	const std::string& GetLanguage() const;

	// Получить префикс для локальной папки в base/locale/ например: de/, fr/, it/, es/ и т.д.
	// Файлы из такой папки грузятся вместо файлов из base.
	// Работает при _useMUI == true.
	// По умолчанию возвращает пустую строку.
	const std::string& GetSystemLanguagePrefix() const;

#ifdef ENGINE_TARGET_WIN32
	//
	// Кодировка для трансляции ввода с клпавиатуры.
	// 1251 - кириллица, 1252 - Latin 1 и т.д.
	//
	void SetCodepage(UINT codepage);
	UINT GetCodepage() const;

	void SetSupportedLanguages(const std::string &suppLangs);
#endif

private:

	bool _ignoreSpaces;

#ifdef ENGINE_TARGET_WIN32
	UINT _codepage;
#endif

	bool _useMui;

	std::string _muiPrefix;

	std::string _language;

#ifdef ENGINE_TARGET_WIN32
	typedef std::set<std::string> Languages;

	Languages _supportedLangs;

	std::string SelectLanguageFromSupported();
#endif

};

extern Locale locale;

}

#endif
