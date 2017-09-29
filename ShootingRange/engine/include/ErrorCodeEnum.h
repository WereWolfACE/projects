#ifndef __ERRORCODEENUM_H__
#define __ERRORCODEENUM_H__

#pragma once

#include <system_error>

#include <boost/preprocessor.hpp>

/*
ЗАДАНИЕ ДАННЫХ ПЕРЕЧИСЛЕНИЯ С КОДАМИ И ОПИСАНИЯМИ ОШИБОК:

Использование X-Macros позволяет задавать имена и описания перечислений в одном месте
и иметь автоматическую генерацию метода для получения строкового описания в коде, а также
избежать ошибок рассинхронизации значений и описаний благодаря отсутствию необходимости
править код в нескольких местах при добавлении или удалении значений.

#define ERROR_CODE_ENUM \
	(ErrorCode, \
	((Success, "success")) \
	((Error1, "error 1 description")) \
	((Error2, "error 2 description")) \
	((Error3, "error 3 description")) \
	((Error4, "error 4 description")) \
	((Error5, "error 5 description")))

ПРИМЕЧАНИЕ:

Поскольку значение 0 в std::error_code всегда считается отсутствием ошибки,
то первым значением перечисления должен быть код, означающий отсутствие ошибки,
т.к. числовые значения не присваиваются явно и первым всегда будет 0.

// В .h файле
DECLARE_ERROR_CODE_ENUM(ERROR_CODE_ENUM)

// В .cpp файле
IMPLEMENT_ERROR_CODE_ENUM(ERROR_CODE_ENUM)

// ИСПОЛЬЗОВАНИЕ:

void some_function(std::error_code& ec) {
	...
	ec = ErrorCode::Error3;
}

std::error_code ec;	
some_function(ec);

if (ec) {
	(1) puts(ec.message().c_str());

	try {
		(2.1) throw std::system_error(ec, "some_function() returned error!");
	} catch (const std::exception& ex) {
		(2.2) puts(ex.what());
	}
}

*/

// Декларирование перечисления с кодами ошибок
#define DECLARE_ERROR_CODE_ENUM_HELPER(_Name, _Enumerators) \
	enum class _Name { \
		BOOST_PP_SEQ_FOR_EACH(DEFINE_ENUM_WITH_DESCRIPTION_VALUE, _, _Enumerators) \
	}; \
	/* Метод MyEnumToString() */ \
	const char* BOOST_PP_CAT(_Name, ToString)(_Name e); \
	/* Поддержка std::error_code */ \
	std::error_code make_error_code(_Name e); \
	std::error_condition make_error_condition(_Name e); \
	namespace std { \
		template <> struct is_error_code_enum<_Name> : public true_type{}; \
	}

// Реализация средств поддержки std::error_code
#define IMPLEMENT_ERROR_CODE_ENUM_HELPER(_Name, _Enumerators) \
	const char* BOOST_PP_CAT(_Name, ToString)(_Name e) { \
		switch (e) { \
			BOOST_PP_SEQ_FOR_EACH(DEFINE_ENUM_WITH_DESCRIPTION_TOSTRING_CASE, _Name, _Enumerators) \
			default: return "Unknown " BOOST_PP_STRINGIZE(_Name) " value"; \
		} \
	} \
	class BOOST_PP_CAT(_Name, CategoryImpl) : public std::error_category { \
	public: \
		virtual const char* name() const BOOST_NOEXCEPT override { return BOOST_PP_STRINGIZE(_Name); } \
		virtual std::string message(int ev) const override { return BOOST_PP_CAT(_Name, ToString)((_Name)ev); } \
	}; \
	const std::error_category& BOOST_PP_CAT(_Name, Category)() { \
		static BOOST_PP_CAT(_Name, CategoryImpl) instance; \
		return instance; \
	} \
	std::error_code make_error_code(_Name e) { return std::error_code(static_cast<int>(e), BOOST_PP_CAT(_Name, Category)()); } \
	std::error_condition make_error_condition(_Name e) { return std::error_condition(static_cast<int>(e), BOOST_PP_CAT(_Name, Category)()); }

#define DEFINE_ENUM_WITH_DESCRIPTION_VALUE(r, data, elem) \
	BOOST_PP_TUPLE_ELEM(2, 0, elem),

#define DEFINE_ENUM_WITH_DESCRIPTION_TOSTRING_CASE(r, data, elem) \
	case data::BOOST_PP_TUPLE_ELEM(2, 0, elem): return BOOST_PP_TUPLE_ELEM(2, 1, elem);

// Макросы для декларирования и реализации перечисления с кодами ошибок
#define DECLARE_ERROR_CODE_ENUM(ENUM) DECLARE_ERROR_CODE_ENUM_HELPER(BOOST_PP_TUPLE_ELEM(2, 0, ENUM), BOOST_PP_TUPLE_ELEM(2, 1, ENUM))
#define IMPLEMENT_ERROR_CODE_ENUM(ENUM) IMPLEMENT_ERROR_CODE_ENUM_HELPER(BOOST_PP_TUPLE_ELEM(2, 0, ENUM), BOOST_PP_TUPLE_ELEM(2, 1, ENUM))

#endif // __ERRORCODEENUM_H__
