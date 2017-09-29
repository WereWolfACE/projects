#ifndef __CORE_GLOBALCONSTANTS__
#define __CORE_GLOBALCONSTANTS__

namespace Core {

// Оболчка для чтения из settings.plist (iOS) и Resources (Android)
class GlobalConstants
{
public:
	static void Init(const std::string &filename);

	static bool NameIsDefined(const std::string &name);

	static bool GetBool(const std::string &name, bool def = false);
	static int GetInt(const std::string &name, int def = 0);
	static float GetFloat(const std::string &name, float def = 0);
	static std::string GetString(const std::string &name, const std::string &def = "");

	// TODO массивы и т.д.

};

} // namespace Core

#endif // __CORE_GLOBALCONSTANTS__
