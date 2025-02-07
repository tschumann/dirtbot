#ifndef __RCBOT_KEY_VAL__
#define __RCBOT_KEY_VAL__

enum : std::uint16_t
{
	RCBOT_MAX_KV_LEN = 256
};

#include <vector>

class CRCBotKeyValue
{
public:
	CRCBotKeyValue(const char *szKey, const char *szValue);

	char *getKey ()
	{
		return m_szKey;
	}

	char *getValue ()
	{
		return m_szValue;
	}

private:
	char m_szKey[RCBOT_MAX_KV_LEN];
	char m_szValue[RCBOT_MAX_KV_LEN];
};

class CRCBotKeyValueList
{
public:
	~CRCBotKeyValueList();

	void parseFile(std::fstream& fp);

	//unsigned int size ();

	//CRCBotKeyValue *getKV ( unsigned int iIndex );

	bool getInt ( const char *key, int *val ) const;

	bool getString ( const char *key, const char **val ) const;

	bool getFloat ( const char *key, float *val ) const;

private:

	CRCBotKeyValue *getKV ( const char *key ) const;

	std::vector <CRCBotKeyValue*> m_KVs;
};

#endif