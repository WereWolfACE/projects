#ifndef UTILS_SELECTOR_H_INCLUDED
#define UTILS_SELECTOR_H_INCLUDED

namespace Render {
	class Texture;
}
namespace utils {

class TextureSelector
{
public:
	typedef std::list<Render::Texture*> TextureList;

	// Конструктор по умолчанию - создает "нулевой" прямоугольник
	explicit TextureSelector( const std::string& format);

	void select( const TextureList& src,  TextureList& res);

	void print( const TextureList& src, std::string &output);

	bool operator()(const Render::Texture* t1, const Render::Texture* t2);

private:

	std::string print_format;
	std::vector<std::string> conditions;
	std::vector<std::string> sort_fields;


	void parsePrint( const std::string &formar );
	void parseWhere( const std::string &formar );
	void parseSort( const std::string &formar );

	bool test( const Render::Texture* , const std::string &condition );
	bool less( const std::string &val1, const std::string &val2);

	std::string tex_print( const Render::Texture * tex, const std::string& format );


	std::string val( const Render::Texture* tex, const std::string &val);

	void where( const TextureList& src,  TextureList& res );


	void sort( TextureList& list );

};
}
#endif //UTILS_SELECTOR_H_INCLUDED