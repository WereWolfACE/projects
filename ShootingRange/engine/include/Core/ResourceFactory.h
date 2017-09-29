#ifndef _RESOURCEFACTORY_H_
#define _RESOURCEFACTORY_H_

/*
 *  ResourceFactory.h
 *  Engine
 *
 */
namespace Render {
	class Texture;
}

/**
 * Наследуйте фабрику ресурсов, чтобы управлять созданием ресурсов.
 * Например, можно создавать свои анимации и текстуры, наследованные от стандартных.
 * 
 * @see resourceFactory = new ResourceFactory() в конструкторе Application.
 */
class ResourceFactory
{
public:
	ResourceFactory();

	virtual ~ResourceFactory() {}

	void EnableCombinedTextures(bool enable) { _enableCombinedTextures = enable; }

	bool CombinedTexturesEnabled() const { return _enableCombinedTextures; }

	virtual void LoadResourcesXml(const std::string& filename);

	virtual void LoadFontsSwl(const std::string& itemId);

	virtual void LoadTexts(const std::string& filename, bool reload = false);

	virtual Render::Texture* CreateTexture(const std::string& filename);

private:
	static void CommonParseResourceNode(
		rapidxml::xml_node<> &node,
		bool baseNoUpload, bool baseForceUpload, const std::string &baseGroup,
		bool &upload, std::set<std::string> &groups);

private:
	bool _enableCombinedTextures;
};

namespace Core {
	extern ResourceFactory* resourceFactory;
}

#endif // _RESOURCEFACTORY_H_
