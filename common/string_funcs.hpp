#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <string>



inline std::string tolower(std::string oldString)
{
	std::string newString = oldString;

	for (size_t index = 0; index < oldString.size(); ++index)
		newString[index] = tolower(oldString[index]);

	return newString;
}

inline std::string toupper(std::string oldString)
{
	std::string newString = oldString;

	for (size_t index = 0; index < oldString.size(); ++index)
		newString[index] = toupper(oldString[index]);

	return newString;
}

inline std::string& tolower_ref(std::string& oldString)
{
	for (size_t index = 0; index < oldString.size(); ++index)
		oldString[index] = tolower(oldString[index]);

	return oldString;
}

inline std::string& toupper_ref(std::string& oldString)
{
	for (size_t index = 0; index < oldString.size(); ++index)
		oldString[index] = toupper(oldString[index]);

	return oldString;
}



#endif /* STRING_FUNCS_H */



