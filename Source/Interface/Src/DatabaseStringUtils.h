/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___DATABASE_STRING_UTILS_H___
#define ___DATABASE_STRING_UTILS_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/*!
	\author Sylvain DOREMUS
	\version	0.7.0.0
	\date		19/06/2012
	\~english
	\brief 		String functions class
	\~french
	\brief 		Classe regroupant des fonctions sur les chaînes de caractères
	*/
	class DatabaseExport CStrUtils
	{
	public:
		/**
		 *\~english
		 *\brief		Tests if the given String represents an integer
		 *\param[in]	p_strToTest	The String to test
		 *\param[in]	p_locale	The locale (unused)
		 *\return		\p false if p_strToTest contains any character other than '0123456789'
		 *\~french
		 *\brief		Teste si le String donné réprésente un nombre entier
		 *\param[in]	p_strToTest Le String à tester
		 *\param[in]	p_locale	La locale (inutilisée)
		 *\return		\p false si p_strToTest contient n'importe quel caractère autre que '0123456789'
		 */
		static bool IsInteger( String const & p_strToTest, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Tests if the given String represents a floating number
		 *\param[in]	p_strToTest	The String to test
		 *\param[in]	p_locale	The locale used to determine the dot format
		 *\return		\p false if p_strToTest contains any character other than '0123456789.,' or more than one '.'/','
		 *\~french
		 *\brief		Teste si le String donné réprésente un nombre flottant
		 *\param[in]	p_strToTest Le String à tester
		 *\param[in]	p_locale	La locale utilisée pour déterminer le format du séparateur '.'
		 *\return		\p false si p_strToTest contient n'importe quel caractère autre que '0123456789.,' ou plus d'un '.'/','
		 */
		static bool IsFloating( String const & p_strToTest, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Tests if the given String represents a date
		 *\remark		Checks dates like 'jj/mm/aa', 'jj/mm/aaaa', 'aa-mm-jj', 'aaaa-mm-jj'
		 *\param[in]	p_strToTest	The String to test
		 *\param[in]	p_locale	The locale used to determine which date format is used
		 *\return		\p false if p_strToTest contains any character other than '0123456789'
		 *\~french
		 *\brief		Teste si le String donné réprésente une date
		 *\remark		Vérifie les dates au format 'jj/mm/aa', 'jj/mm/aaaa', 'aa-mm-jj', 'aaaa-mm-jj'
		 *\param[in]	p_strToTest Le String à tester
		 *\param[in]	p_locale	La locale permettant de déterminer quel format de date est utilisé
		 *\return		\p false si p_strToTest contient n'importe quel caractère autre que '0123456789'
		 */
		static bool IsDate( String const & p_strToTest, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a short from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The short contained in the String
		 *\~french
		 *\brief		Récupère un short à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le short contenu dans le String
		 */
		static short ToShort( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves an int from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The int contained in the String
		 *\~french
		 *\brief		Récupère un int à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		L'int contenu dans le String
		 */
		static int ToInt( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a long from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The long contained in the String
		 *\~french
		 *\brief		Récupère un long à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le long contenu dans le String
		 */
		static long ToLong( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a long long from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The long long contained in the String
		 *\~french
		 *\brief		Récupère un long long à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le long long contenu dans le String
		 */
		static long long ToLongLong( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a float from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The float contained in the String
		 *\~french
		 *\brief		Récupère un float à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le float contenu dans le String
		 */
		static float ToFloat( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a double from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The double contained in the String
		 *\~french
		 *\brief		Récupère un double à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le double contenu dans le String
		 */
		static double ToDouble( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Retrieves a double from a given String
		 *\param[in]	p_str		The String
		 *\param[in]	p_locale	The locale used in the conversion
		 *\return		The double contained in the String
		 *\~french
		 *\brief		Récupère un double à partir d'un String
		 *\param[in]	p_str		Un String
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\return		Le double contenu dans le String
		 */
		static long double ToLongDouble( String const & p_str, std::locale const & p_locale = std::locale() );
		/**
		 *\~english
		 *\brief		Tests if the given String is upper case
		 *\param[in]	p_strToTest	The String to test
		 *\return		\p true if p_strToTest is upper case
		 *\~french
		 *\brief		Teste si le String donné est en majuscules
		 *\param[in]	p_strToTest	Le String à tester
		 *\return		\p true si p_strToTest est en majuscules
		 */
		static bool IsUpperCase( std::string const & p_strToTest );
		/**
		 *\~english
		 *\brief		Tests if the given String is lower case
		 *\param[in]	p_strToTest	The String to test
		 *\return		\p true if p_strToTest is lower case
		 *\~french
		 *\brief		Teste si le String donné est en minuscules
		 *\param[in]	p_strToTest	Le String à tester
		 *\return		\p true si p_strToTest est en minuscules
		 */
		static bool IsLowerCase( std::string const & p_strToTest );
		/**
		 *\~english
		 *\brief		Tests if the given String is upper case
		 *\param[in]	p_strToTest	The String to test
		 *\return		\p true if p_strToTest is upper case
		 *\~french
		 *\brief		Teste si le String donné est en majuscules
		 *\param[in]	p_strToTest	Le String à tester
		 *\return		\p true si p_strToTest est en majuscules
		 */
		static bool IsUpperCase( std::wstring const & p_strToTest );
		/**
		 *\~english
		 *\brief		Tests if the given String is lower case
		 *\param[in]	p_strToTest	The String to test
		 *\return		\p true if p_strToTest is lower case
		 *\~french
		 *\brief		Teste si le String donné est en minuscules
		 *\param[in]	p_strToTest	Le String à tester
		 *\return		\p true si p_strToTest est en minuscules
		 */
		static bool IsLowerCase( std::wstring const & p_strToTest );
		/**
		 *\~english
		 *\brief		Sets all characters in a String to upper case
		 *\param[in]	p_str	The String to modify
		 *\return		The modified String
		 *\~french
		 *\brief		Met tous les caractères du String donné en majuscules
		 *\param[in]	p_str	Le String à modifier, reçoit le String modifié
		 *\return		Le String modifié
		 */
		static std::string UpperCase( std::string const & p_str );
		/**
		 *\~english
		 *\brief		Sets all characters in a String to lower case
		 *\param[in]	p_str	The String to modify
		 *\return		The modified String
		 *\~french
		 *\brief		Met tous les caractères du String donné en minuscules
		 *\param[in]	p_str	Le String à modifier, reçoit le String modifié
		 *\return		Le String modifié
		 */
		static std::string LowerCase( std::string const & p_str );
		/**
		 *\~english
		 *\brief		Sets all characters in a String to upper case
		 *\param[in]	p_str	The String to modify
		 *\return		The modified String
		 *\~french
		 *\brief		Met tous les caractères du String donné en majuscules
		 *\param[in]	p_str	Le String à modifier, reçoit le String modifié
		 *\return		Le String modifié
		 */
		static std::wstring UpperCase( std::wstring const & p_str );
		/**
		 *\~english
		 *\brief		Sets all characters in a String to lower case
		 *\param[in]	p_str	The String to modify
		 *\return		The modified String
		 *\~french
		 *\brief		Met tous les caractères du String donné en minuscules
		 *\param[in]	p_str	Le String à modifier, reçoit le String modifié
		 *\return		Le String modifié
		 */
		static std::wstring LowerCase( std::wstring const & p_str );
		/**
		 *\~english
		 *\brief			Sets all characters in a String to upper case
		 *\param[in,out]	p_str	The String to modify, receives the modified String
		 *\return			The modified String
		 *\~french
		 *\brief			Met tous les caractères du String donné en majuscules
		 *\param[in,out]	p_str	Le String à modifier, reçoit le String modifié
		 *\return			Le String modifié
		 */
		static std::string & ToUpperCase( std::string & p_str );
		/**
		 *\~english
		 *\brief			Sets all characters in a String to lower case
		 *\param[in,out]	p_str	The String to modify, receives the modified String
		 *\return			The modified String
		 *\~french
		 *\brief			Met tous les caractères du String donné en minuscules
		 *\param[in,out]	p_str	Le String à modifier, reçoit le String modifié
		 *\return			Le String modifié
		 */
		static std::string & ToLowerCase( std::string & p_str );
		/**
		 *\~english
		 *\brief			Sets all characters in a String to upper case
		 *\param[in,out]	p_str	The String to modify, receives the modified String
		 *\return			The modified String
		 *\~french
		 *\brief			Met tous les caractères du String donné en majuscules
		 *\param[in,out]	p_str	Le String à modifier, reçoit le String modifié
		 *\return			Le String modifié
		 */
		static std::wstring & ToUpperCase( std::wstring & p_str );
		/**
		 *\~english
		 *\brief			Sets all characters in a String to lower case
		 *\param[in,out]	p_str	The String to modify, receives the modified String
		 *\return			The modified String
		 *\~french
		 *\brief			Met tous les caractères du String donné en minuscules
		 *\param[in,out]	p_str	Le String à modifier, reçoit le String modifié
		 *\return			Le String modifié
		 */
		static std::wstring & ToLowerCase( std::wstring & p_str );
		/**
		 *\~english
		 *\brief		Retrieves a std::string from a String
		 *\param[in]	p_str		The String
		 *\return		The std::string
		 *\~french
		 *\brief		Récupère un std::string à partir d'un String
		 *\param[in]	p_str		Le String
		 *\return		Le std::wstring
		 */
		static std::string ToStr( std::string const & p_str )
		{
			return p_str;
		}
		/**
		 *\~english
		 *\brief		Retrieves a std::wstring from a String
		 *\param[in]	p_str		The String
		 *\return		The std::string
		 *\~french
		 *\brief		Récupère un std::wstring à partir d'un String
		 *\param[in]	p_str		Le String
		 *\return		Le std::wstring
		 */
		static std::wstring ToWStr( std::string const & p_str );
		/**
		 *\~english
		 *\brief		Retrieves a std::string from a String
		 *\param[in]	p_str		The String
		 *\return		The std::string
		 *\~french
		 *\brief		Récupère un std::string à partir d'un String
		 *\param[in]	p_str		Le String
		 *\return		Le std::wstring
		 */
		static std::string ToStr( std::wstring const & p_str );
		/**
		 *\~english
		 *\brief		Retrieves a std::wstring from a String
		 *\param[in]	p_str		The String
		 *\return		The std::string
		 *\~french
		 *\brief		Récupère un std::wstring à partir d'un String
		 *\param[in]	p_str		Le String
		 *\return		Le std::wstring
		 */
		static std::wstring ToWStr( std::wstring const & p_str )
		{
			return p_str;
		}
		/**
		 *\~english
		 *\brief		Cuts a String into substrings, using delimiter(s)
		 *\param[in]	p_str		The String to cut
		 *\param[in]	p_delims	The delimiter(s)
		 *\param[in]	p_maxSplits	The max splits count (the return will contain 0 < x < p_maxSplits substrings)
		 *\param[in]	p_bKeepVoid	Tells if the function keeps void substrings or not
		 *\return		The array containing the substrings
		 *\~french
		 *\brief		Découpe une chaîne en plusieurs sous-chaînes, en utilisant un/des délimiteur(s)
		 *\param[in]	p_str		Le String à découper
		 *\param[in]	p_delims	Le(s) délimiteur(s)
		 *\param[in]	p_maxSplits	Le nombre maximal de découpes (le retour contiendra 0 < x < p_maxSplits sous-chaînes)
		 *\param[in]	p_bKeepVoid	Dit si la fonction garde les sous-chaînes vides ou pas
		 *\return		Le tableau contenant les sous-chaînes
		 */
		static StringArray Split( String const & p_str, String const & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );
		/**
		 *\~english
		 *\brief		Retrieves a value from the given String
		 *\param[in]	p_str		The String supposedly containing the value
		 *\param[out]	p_val		Receives the retrieved value
		 *\param[in]	p_locale	The locale used in the conversion
		 *\~french
		 *\brief
		 *\brief		Récupère une valeur à partir d'un String
		 *\param[in]	p_str	Le String contenant normalement la valeur
		 *\param[out]	p_val	Reçoit la valeur récupérée
		 *\return
		 */
		template <typename T>
		static inline void Parse( String const & p_str, T & p_val )
		{
			StringStream l_out( p_str );
			l_out >> p_val;
		}
		/**
		 *\~english
		 *\brief		Retrieves a value from the given String
		 *\param[in]	p_str		The String supposedly containing the value
		 *\param[in]	p_locale	The locale used in the conversion
		 *\param[out]	p_val		Receives the retrieved value
		 *\~french
		 *\brief
		 *\brief		Récupère une valeur à partir d'un String
		 *\param[in]	p_str		Le String contenant normalement la valeur
		 *\param[in]	p_locale	La locale utilisée dans la conversion
		 *\param[out]	p_val		Reçoit la valeur récupérée
		 *\return
		 */
		template <typename T>
		static inline void Parse( String const & p_str, std::locale const & p_locale, T & p_val )
		{
			StringStream l_out( p_str );
			l_out.imbue( p_locale );
			l_out >> p_val;
		}
		/**
		 *\~english
		 *\brief			Replaces all occurences of a TChar by another one in a String
		 *\param[in,out]	p_str			The String to modify
		 *\param[in]		p_find			The TChar to replace
		 *\param[in]		p_replacement	The replacement TChar
		 *\return			A reference on the modified string
		 *\~french
		 *\brief			Remplace toutes les occurences d'un TChar par un autre dans un String
		 *\param[in,out]	p_str			Le String à modifier
		 *\param[in]		p_find			Le TChar à remplacer
		 *\param[in]		p_replacement	Le TChar de remplacement
		 *\return			Une référence sur la chaîne modifiée
		 */
		static String & Replace( String & p_str, TChar p_find, TChar p_replacement );
		/**
		 *\~english
		 *\brief			Replaces all occurences of a String by a TChar in a String
		 *\param[in,out]	p_str			The String to modify
		 *\param[in]		p_find			The String to replace
		 *\param[in]		p_replacement	The replacement TChar
		 *\return			A reference on the modified string
		 *\~french
		 *\brief			Remplace toutes les occurences d'un String par un TChar dans un String
		 *\param[in,out]	p_str			Le String à modifier
		 *\param[in]		p_find			Le String à remplacer
		 *\param[in]		p_replacement	Le TChar de remplacement
		 *\return			Une référence sur la chaîne modifiée
		 */
		static String & Replace( String & p_str, String const & p_find, TChar p_replacement );
		/**
		 *\~english
		 *\brief			Replaces all occurences of a TChar by a String in a String
		 *\param[in,out]	p_str			The String to modify
		 *\param[in]		p_find			The TChar to replace
		 *\param[in]		p_replacement	The replacement String
		 *\return			A reference on the modified string
		 *\~french
		 *\brief			Remplace toutes les occurences d'un TChar par un String dans un String
		 *\param[in,out]	p_str			Le String à modifier
		 *\param[in]		p_find			Le TChar à remplacer
		 *\param[in]		p_replacement	Le String de remplacement
		 *\return			Une référence sur la chaîne modifiée
		 */
		static String & Replace( String & p_str, TChar p_find, String const & p_replacement );
		/**
		 *\~english
		 *\brief			Replaces all occurences of a String in another one by a third one
		 *\param[in,out]	p_str			The String to modify
		 *\param[in]		p_find			The String to replace
		 *\param[in]		p_replacement	The replacement String
		 *\return			A reference on the modified string
		 *\~french
		 *\brief			Remplace toutes les occurences d'un String par un autre dans un troisième
		 *\param[in,out]	p_str			Le String à modifier
		 *\param[in]		p_find			Le String à remplacer
		 *\param[in]		p_replacement	Le String de remplacement
		 *\return			Une référence sur la chaîne modifiée
		 */
		static String & Replace( String & p_str, String const & p_find, String const & p_replacement );
		/**
		 *\~english
		 *\brief			Removes spaces on the left and/or on the right of the given String
		 *\param[in,out]	p_str		The String to trim, receives the trimmed string
		 *\param[in]		p_bLeft		Tells if we remove the left spaces
		 *\param[in]		p_bRight	Tells if we remove the right spaces
		 *\return			The trimmed String
		 *\~french
		 *\brief			Supprime les espaces à gauche et/ou à droite dans la chaîne donnée
		 *\param[in,out]	p_str		La chaîne à modifier, reçoit la chaîne modifiée
		 *\param[in]		p_bLeft		Dit si on enlève les espaces à gauche
		 *\param[in]		p_bRight	Dit si on enlève les espaces à droite
		 *\return			La chaîne sans espaces
		 */
		static String & Trim( String & p_str, bool p_bLeft = true, bool p_bRight = true );
		/**
		 *\~english
		 *\brief		Puts a value into a String
		 *\param[in]	p_tValue	The value
		 *\return		The String containing the value
		 *\~french
		 *\brief		Met une valeur dans un String
		 *\param[in]	p_tValue	La valeur
		 *\return		Le String contenant la valeur
		 */
		template< typename T >
		static String ToString( T const & p_tValue )
		{
			StringStream l_streamReturn;
			l_streamReturn <<  p_tValue;
			return l_streamReturn.str();
		}
		/**
		 *\~english
		 *\brief		Sets the String value to the given std::string
		 *\param[in]	p_strText	The std::string
		 *\return		The String
		 *\~french
		 *\brief		Définit la valeur du String à celle du std::string
		 *\param[in]	p_strText	Le std::string
		 *\return		Le String
		 */
		static String ToString( std::string const & p_strText );
		/**
		 *\~english
		 *\brief		Sets the String value to the given std::wstring
		 *\param[in]	p_strText	The std::wstring
		 *\return		The String
		 *\~french
		 *\brief		Définit la valeur du String à celle du std::wstring
		 *\param[in]	p_strText	Le std::wstring
		 *\return		Le String
		 */
		static String ToString( std::wstring const & p_strText );
		/**
		 *\~english
		 *\brief		Sets the String value to the given char
		 *\param[in]	p_char	The char
		 *\return		The String
		 *\~french
		 *\brief		Définit la valeur du String à celle du char
		 *\param[in]	p_char	Le char
		 *\return		Le String
		 */
		static String ToString( char p_char );
		/**
		 *\~english
		 *\brief		Sets the String value to the given wchar_t
		 *\param[in]	p_wchar	The wchar_t
		 *\return		The String
		 *\~french
		 *\brief		Définit la valeur du String à celle du wchar_t
		 *\param[in]	p_wchar	Le wchar_t
		 *\return		Le String
		 */
		static String ToString( wchar_t p_wchar );

	private:
		static int DoGetMonthIndex( String const & strMonth );
		static bool DoIsValidDay( int iDay, int iMonth, int iYear );
	};
	/*!
	\author 	Sylvain DOREMUS
	\date 		19/06/2012
	\~english
	\brief		An empty String
	\~french
	\brief		Une chaîne vide
	*/
	static const String cuEmptyString;
}
END_NAMESPACE_DATABASE
/**
 *\~english
 *\brief			Stream operator, using String
 *\param[in,out]	p_strStream	The String stream
 *\param[in]		p_tVal		The value to put in the stream
 *\return			The String stream
 *\~french
 *\brief			Opérateur de flux, utilisant String
 *\param[in,out]	p_strStream	Le flux String
 *\param[in]		p_tVal		La valeur à mettre dans le flux
 *\return			Le flux String
 */
template <typename T>
inline NAMESPACE_DATABASE::String & operator <<( NAMESPACE_DATABASE::String & p_strStream, T const & p_tVal )
{
	Castor::StringStream l_out;
	l_out << p_tVal;
	p_strStream += l_out.str();
	return p_strStream;
}
/**
 *\~english
 *\brief			Stream operator, using String
 *\param[in,out]	p_strStream	The String stream
 *\param[in]		p_strVal	The value to put in the stream
 *\return			The String stream
 *\~french
 *\brief			Opérateur de flux, utilisant String
 *\param[in,out]	p_strStream	Le flux String
 *\param[in]		p_strVal	La valeur à mettre dans le flux
 *\return			Le flux String
 */
inline NAMESPACE_DATABASE::String & operator <<( NAMESPACE_DATABASE::String & p_strStream, NAMESPACE_DATABASE::String const & p_strVal )
{
	p_strStream += p_strVal;
	return p_strStream;
}
/**
 *\~english
 *\brief			Stream operator, using String
 *\param[in,out]	p_strStream	The String stream
 *\param[in]		p_tVal		The value to get from the stream
 *\return			The String stream
 *\~french
 *\brief			Opérateur de flux, utilisant String
 *\param[in,out]	p_strStream	Le flux String
 *\param[in]		p_tVal		La valeur à récupérer à partir du flux
 *\return			Le flux String
 */
template <typename T>
inline NAMESPACE_DATABASE::String & operator >>( NAMESPACE_DATABASE::String & p_strStream, T & p_tVal )
{
	NAMESPACE_DATABASE::StringStream l_out( p_strStream );
	l_out >> p_tVal;
	p_strStream = l_out.str();
	return p_strStream;
}

#endif
