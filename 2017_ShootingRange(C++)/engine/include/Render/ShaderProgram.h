#ifndef __RENDER_SHADERPROGRAM_H__
#define __RENDER_SHADERPROGRAM_H__

#include "Core/Resource.h"
#include "Render/RestorableObject.h"
#include "Utils/Matrix4.h"

class ShaderProgramLoader;

namespace Render {

/// Класс для управления вершинными и пиксельными шейдерами.
class ShaderProgram : public Resource, public RestorableObject {
public:
	static ShaderProgram* CreateFromXml(rapidxml::xml_node<>* elem);
	static ShaderProgram* CreateFromFile(const std::string& filenameVS, const std::string& filenamePS);
	static ShaderProgram* CreateFromSource(const std::string& sourceVS, const std::string& sourcePS);

	ShaderProgram();

	virtual ~ShaderProgram();

	const std::string& GetName() const { return _name; }

	void SetName(const std::string& name) { _name = name; }

	/// Устанавливает файлы исходника шейдера
	void SetFileNames(const std::string& filenameVS, const std::string& filenamePS) { _filenameVS = filenameVS; _filenamePS = filenamePS; _isSource = false; }

	/// Устанавливает текст исходника шейдера
	void SetSource(const std::string& sourceVS, const std::string& sourcePS) { _filenameVS = sourceVS; _filenamePS = sourcePS; _isSource = true; }

	/// Добавляет символ препроцессора
	void AddDefine(const std::string& define) { _defines.push_back("#define " + define + "\n"); }

	void ClearDefines() { Strings().swap(_defines); }
	
	/// Создание пары шейдеров из файлов. Возможно задание пустой строки вместо одного из шейдеров
	virtual bool LoadFile(const std::string& filenameVS, const std::string& filenamePS) = 0;

	/// Создание пары шейдеров из строк с программой. Возможно задание пустой строки вместо одного из шейдеров
	virtual bool LoadSource(const std::string& sourceVS, const std::string& sourcePS) = 0;
	
	/// Связывание шейдеров с устройством
	virtual void Bind() = 0;
	
	/// Отвязывание шейдеров от устройства
	virtual void Unbind() = 0;

	/// Возвращает идентификатор программы
	virtual void* GetProgram() const = 0;
	
	/// Имеется ли данный параметр
	virtual bool HasUniform(const std::string& name) const = 0;

	/// Установка параметров
	virtual void SetUniform(const std::string& name, const float* value, int size) = 0;
	virtual void SetUniform(const std::string& name, const int* value, int size) = 0;

	void SetUniform(const std::string& name, float value)                                            { SetUniform(name, &value, 1); }
	void SetUniform(const std::string& name, float value1, float value2)                             { float values[2] = { value1, value2 }; SetUniform(name, values, 2); }
	void SetUniform(const std::string& name, float value1, float value2, float value3)               { float values[3] = { value1, value2, value3 }; SetUniform(name, values, 3); }
	void SetUniform(const std::string& name, float value1, float value2, float value3, float value4) { float values[4] = { value1, value2, value3, value4 }; SetUniform(name, values, 4); }

	void SetUniform(const std::string& name, int value)                                      { SetUniform(name, &value, 1); }
	void SetUniform(const std::string& name, int value1, int value2)                         { int values[2] = { value1, value2 }; SetUniform(name, values, 2); }
	void SetUniform(const std::string& name, int value1, int value2, int value3)             { int values[3] = { value1, value2, value3 }; SetUniform(name, values, 3); }
	void SetUniform(const std::string& name, int value1, int value2, int value3, int value4) { int values[4] = { value1, value2, value3, value4 }; SetUniform(name, values, 4); }

	void SetUniform(const std::string& name, const math::Matrix4& value) { SetUniform(name, value.v, 16); }

protected:
	friend class ::ShaderProgramLoader;

	/// Загрузка шейдеров в устройство
	virtual void Upload() { _isSource ? LoadSource(_filenameVS, _filenamePS) : LoadFile(_filenameVS, _filenamePS); }
	
	/// Выгрузка шейдеров из устройства
	virtual void Release() = 0;

protected:
	std::string _name;
	std::string _filenameVS;
	std::string _filenamePS;
	bool _isSource;

	typedef std::vector<std::string> Strings;

	Strings _defines;
};

typedef boost::intrusive_ptr<ShaderProgram> ShaderProgramPtr;

} // namespace Render

#endif // __RENDER_SHADERPROGRAM_H__
