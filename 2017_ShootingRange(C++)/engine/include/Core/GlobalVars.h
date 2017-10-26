#ifndef __CORE_GLOBALVARS__
#define __CORE_GLOBALVARS__

namespace Core {

// Оболочка для работы с NSUserDefaults (iOS) и SharedPreferences (Android)
class GlobalVars
{
public:
	static bool NameIsDefined(const std::string &name);

	static void Sync();

	static bool GetBool(const std::string &name, bool def = false);
	static void SetBool(const std::string &name, bool value);

	static int GetInt(const std::string &name, int def = 0);
	static void SetInt(const std::string &name, int value);

	static int64_t GetInt64(const std::string &name, int64_t def = 0);
	static void SetInt64(const std::string &name, int64_t value);

	static float GetFloat(const std::string &name, float def = 0);
	static void SetFloat(const std::string &name, float value);

	static std::string GetString(const std::string &name, const std::string &def = "");
	static void SetString(const std::string &name, const std::string &value);

	static std::vector<uint8_t> GetBytes(const std::string &name);
	static void SetBytes(const std::string &name, const std::vector<uint8_t> &value);
	
	static void Remove(const std::string &name);
	static void Clear();

	static void SetKey(const std::vector<uint8_t> &key);
	static bool WasEncryptionError();
	static void ResetErrorFlag();

	// TODO массивы и т.д.

};

} // namespace Core

#endif // __CORE_GLOBALVARS__
