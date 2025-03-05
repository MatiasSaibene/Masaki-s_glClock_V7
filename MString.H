
// ======================================================================== //
//                                                                          //
//                         String ���饹����ե�����                        //
//                                                                          //
//          December 1996 by Masaki Kawase (g93088@isc.chubu.ac.jp)         //
//                                                                          //
//                                                                          //
//                               ���饹 String                              //
//                                                                          //
//                                                                          //
//                           ���� ʸ�������饹                          //
//                                                                          //
//                                                                          //
//                                                                          //
//  ��Ϣ�ե����� :                                                          //
//    "String.H" : ���饹������ڤӥ���饤��ؿ�����ʤ��Υե������       //
//                ���ץꥱ������� ���󥯥롼���ѥե�����                   //
//    "String.C"     : ����¾�δؿ����                                     //
//                                                                          //
//  ���󥹥ȥ饯�� :                                                        //
//    String() : �̥�ʸ���ǽ����                                           //
//    String(int n) : n ʸ��Ĺ�Υ��ڡ����ǽ����                            //
//    String(char *c) : ʸ���� c �ǽ����                                   //
//    String(char *c, int n) : ʸ���� c �ǽ������Ĺ���϶���Ū�� n ʸ��     //
//        ;�ä����ϡ����ڡ�������ޤ�                                    //
//    String(String s, int n) : String ���֥������Ȥ�Ȥ��ʳ��Ͼ��Ʊ��     //
//    String(String s) : String ���֥������ȥ��ԡ�                          //
//                                                                          //
//                                                                          //
//  st �� String ���饹�Υ��֥������ȤȤ����String st ; ��                 //
//                                                                          //
//  �᥽�å� :                                                              //
//    int st.Length() : ʸ����Ĺ���֤�                                      //
//    String& st.Length(n) : ʸ����Ĺ�򥻥åȡ�;�ä����ϥ��ڡ�����       //
//                                                                          //
//    st = String, st = (char*) ������                                      //
//                                                                          //
//    �ʹߤΥ᥽�åɤǡ�                                                    //
//    Mid(), Left(), Right() �ϡ�Ĺ�᤮��;�ä���ʬ�ϵͤ����û���ʤ�     //
//    ���󥹥ȥ饯���Ȱ㤤��st ��Ĺ���� ���� n �ˤʤ�Ȥϸ¤�ʤ�           //
//                                                                          //
//    st.Mid(int m, n) : st ��m ʸ���ܤ��� n ʸ��ʬ��ʸ������Ѳ�         //
//    Mid(st, int m, n) : st �Ρ�m ʸ���ܤ��� n ʸ��ʬ����Ф�            //
//                                                                          //
//    st.Left(int n) : st �򡢺����� n ʸ��ʬ��ʸ������Ѳ�                 //
//        Length(n) �Ȥΰ㤤�ϡ�st ����Ϥ߽Ф�����ʬ�ϥ��åȤ���뤳��     //
//        Length(n) �Ǥ� Ĺ����ɬ�� n �ˡ��Ϥ߽Ф�����ʬ�ϥ��ڡ�������ޤ�  //
//    Left(st, n) : st �Ρ������� n ʸ��ʬ����Ф�                        //
//                                                                          //
//    st.Right(int n) : st �򡢱����� n ʸ��ʬ��ʸ������Ѳ�                //
//    Right(st, n) : st �Ρ������� n ʸ��ʬ����Ф�                       //
//                                                                          //
//                                                                          //
//  ��ӱ黻�� :                                                            //
//    ==, !=, <, >, <=, <= : ʸ��������                                   //
//        �����ϡ�ξ���� String �ޤ��ϡ�String �� char* ����              //
//                                                                          //
//  �����ϥ��ڥ졼�� :                                                      //
//    <<, >>                                                                //
//                                                                          //
//                                                                          //
// ======================================================================== //


#ifndef ___STRING_H___
#define ___STRING_H___

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "Define.H"

using namespace std;

//#define STRING_DEBUG

// ���Х��������Ǽ��ޤ� 10 �Τ٤���κ����͡�IntToString() �ǻ��ѡ�
#define MAX_DIV_DEF   1000000000


class String
{
	enum
	{
		MIN_ALLOC_UNIT = 64,
		MAX_ALLOC_UNIT = 4096,
		MAX_LINE_BUF   = 16384,
	} ;

    int     length ;
	int     allocated ;
    char    *str ;

	// ��������Υ��ꥢ��
    inline String& Clear()
	{
		if (str) delete [] str ;
		length = 0 ;
		str = new char[1] ;
		str[0] = '\0' ;
		allocated = 1 ;
		return *this ;
	}

	// n ʸ�����������ºݤ˳��ݤ��٤�������
	// n ����Ū��ʸ����Ĺ
	inline static int shouldAlloc(int n)
	{
		if (n < MIN_ALLOC_UNIT)
			return MIN_ALLOC_UNIT ;
		else
		{
			if ((n + 1) * 2 > MAX_ALLOC_UNIT)
				return n + 1 + MAX_ALLOC_UNIT ;
			else
				return (n + 1) * 2 ;
		}
	}

	// �Ƴ������
	// �Хåե���­��ʤ����
	//   ���ݤ��ƥ��ԡ�
	//
	// ����­��Ƥ�����
	//   ɬ���̤��ܰʾ夢�����
	//   ɬ���̤��ܤޤǺ��
	//   �¥ǡ���Ĺ length �������ǽ��������Τ�����
	//
	// n �ϡ�ɬ�פ�ʸ����Ĺ
	// ���Υ���������������ʸ����Ĺ
	//
	// guarantee �� FALSE �����ꤵ��Ƥ������
	// �ǡ����Υ��ԡ��ϼ¹Ԥ��ʤ�
	inline void Realloc(int n, int guarantee = TRUE)
	{
#ifdef STRING_DEBUG
		cout << "String::Realloc(" << n << ", " << guarantee << ")" << endl ;
#endif
		if (n == length) return ;

		if (n + 1 > allocated)
		{
			// ɬ���̤��ܳ��ݤ���
			// ������ɬ���̤��ܤ� MAX_ALLOC_UNIT ��Ķ������
			// ɬ���̡�MAX_ALLOC_UNIT ʬ���ݤ���
			allocated = shouldAlloc(n) ;
//			if ((n + 1) * 2 > MAX_ALLOC_UNIT)
//				allocated = n + 1 + MAX_ALLOC_UNIT ;
//			else
//				allocated = (n + 1) * 2 ;

			char *w = new char[allocated] ;

			if (guarantee)
			{
				// �¥ǡ����ʤĤޤ�ʸ����Ĺ�ܣ���
				// length + 1 �Х��ȥ��ԡ�
				// ������ length �� n ���¿������ n �������ɤ�
				if (length > n)
				{
					memcpy(w, str, n) ;
					w[n] = '\0' ;
				}
				else
					memcpy(w, str, length + 1) ;
			}

			// �Ǹ�˥Хåե����
			delete [] str ;
			str = w ;
		}
		else
		{
			// �Хåե������ޤ���
			// �ºݤ�ɬ���̤��ܤ�Ķ���Ƥ������
			// ɬ���̤��ܡ�����������Ǥ� MIN_ALLOC_UNIT �˥Хåե�����
			//   �¥ǡ���Ĺ length �������ǽ��������Τ�����
			if ((n + 1) * 2 < allocated)
			{
				// ɬ���̤��ܤޤǥǡ�������
				// ������ɬ���̤��ܤ� MAX_ALLOC_UNIT ��Ķ������
				// ɬ���̡�MAX_ALLOC_UNIT ʬ���ݤ���
				if ((n + 1) * 2 > MAX_ALLOC_UNIT)
					allocated = n + 1 + MAX_ALLOC_UNIT ;
				else if ((n + 1) * 2 < MIN_ALLOC_UNIT)
					allocated = MIN_ALLOC_UNIT ;
				else
					allocated = (n + 1) * 2 ;

				char *w = new char[allocated] ;

				if (guarantee)
				{
					// ���θ�ɬ�פˤʤ�ǡ�����n��
					// ���ʤ�� n �Х��ȥ��ԡ�
					// ���������⤷�����̤� n + 1 ��­��ʤ�����
					// ­�����ʬ�ޤǤ򥳥ԡ�
					//
					// n ʸ���κǸ�ˤϥ̥�ʸ���ɲ�
					if (n + 1 > allocated)
					{
						memcpy(w, str, allocated - 1) ;
						w[allocated - 1] = '\0' ;
					}
					else
					{
						memcpy(w, str, n) ;
						w[n] = '\0' ;
					}
				}

				// �Ǹ�˥Хåե����
				delete [] str ;
				str = w ;
			}
		}
	}

	// �ǡ����Υ��ԡ�
	// ���ԡ���򸵤˺Ƴ��ݤ��Ƥ���ǡ����򥳥ԡ�
	inline void Copy(const String& s)
	{
		if (this == &s) return ;

		// ɬ�פʾ��ϺƳ���
		// ���ʤ��Ȥ⥳�ԡ��� s.length ʬ�Υǡ������ݾڤ����
		// �������ǡ����Υ��ԡ���ɬ�פʤ��Τ� FALSE ���Ϥ�
		Realloc(s.length, FALSE) ;

		// �ºݤΥǡ������ԡ�
		length = s.length ;
		memcpy(str, s.str, s.length) ;

		// �Ǹ�ϰ������Ū�˥̥�ʸ���ɲ�
		str[length] = '\0' ;
	}

	String& ReadAllLines(FILE *fp) ;

  public:

    // ���󥹥ȥ饯��
    String()
	{
		str = NULL ;
		Clear() ;
	}

    String(int n) ;
//    String(char *c, int n = -1) ;
    String(const char *c) ;
    String(const char *c, int n) ;// = -1) ;
    String(const String &s) ;
    String(const String &s, int) ;// n = -1) ;
    String(char c) ;

    // �ǥ��ȥ饯��
    ~String() { if (str) delete [] str ; }

    // ���㥹�ȱ黻�ҡ�char*��
//#ifdef WIN32
	inline operator char*() const { return str ; }
//#else
//	const operator char*() const { return str ; }
//#endif

    // ��������
    inline int Length() const { return length ; }

    // ʸ�����Ĺ�����Ѳ���;�ä���ʬ�ϥ��ڡ��� ' '��
    String& Length(int n) ;

    // n ʸ���ܤΣ�ʸ��
    char Character(int n = 1) const ;

	// n ʸ���ܤ�ʸ�����ѹ�
	String& SetAt(int n, char c) ;

	String& ReplaceString(char s, char d) ;
	String& ReplaceString(const String& s, const String& d) ;
	inline String& ReplaceString(const String& s, char d) { ReplaceString(s, String(d)) ; return *this ; }
	inline String& ReplaceString(const char s, const String& d) { ReplaceString(String(s), d) ; return *this ; }

	// Lower, Upper
	String& Lower() ;
	inline friend String Lower(const String& s) { String w = s ; w.Lower() ; return w ; }
	String& Upper() ;
	inline friend String Upper(const String& s) { String w = s ; w.Upper() ; return w ; }

	// �ե����ޥå�
	String& Format(const char* format, ...) ;

	// �ե����ޥåȤ��ɲ�
	String& AddFormat(const char* format, ...) ;

	// �ƥ����ȥե������ɤ߹���
	String& ReadFile(const String& fileName) ;

	// �ȡ��������
	// �֤��줿�ȡ�����ʺǽ��ʸ����ˤϺ�������
	// String("") ���֤����ޤǷ����֤�����
	String Token(const String& delimiters = String(" \t\n"), const String& comment = String("//")) ;

    // Clase String (presupongo que es parte de la clase String)
	String& Mid(int m, int n = 1); // Aquí se permite el argumento predeterminado
	friend String Mid(String s, int m, int n); // Sin argumento predeterminado

	String& Left(int n = 1); // Aquí se permite el argumento predeterminado
	friend String Left(const String& s, int n); // Sin argumento predeterminado

	String& Right(int n = 1); // Aquí se permite el argumento predeterminado
	friend String Right(const String& s, int n); // Sin argumento predeterminado

	String FileDirectory() const ;
	String FileNamePart() const ;

    // ʸ���󥵡����ʺǽ�˰��פ��������֤���
    int Search(const String& key) const ;
    int Search(const char   *key) const ;
    int Search(const char    key) const ;

    // ���ڥ졼����/�ˤĤޤ�ϡ�������ʸ����򥵡����������פ���������֤�
    int operator /(const String& key) const ;
    int operator /(const char   *key) const ;
    int operator /(const char    key) const ;

    // ���ڥ졼����=�������黻��
    String& operator =(const String& s) ;
    String& operator =(const char   *c) ;
    String& operator =(const char    c) ;

    // ʸ����ӡʺǽ�� n ʸ���� s ��Ʊ���ǡ�n ���夬 s ��Ʊ�������ڤ���
    // ���ޥ�ɥ��ץ����Υ������ʤɤǻ���
    int Equ(const String s, int n = 1) ;

    // n ʸ���羮��ӡ�strncmp()��Ʊ��������n ���ά����ȡ�strcmp() ��Ʊ����
    int Cmp(const String s, int n = -1) ;

    // ���ڥ졼����==, !=����ӱ黻��
    int operator ==(const String& s2) const ;
    int operator !=(const String& s2) const ;
    int operator ==(const char    *c) const ;
    int operator !=(const char    *c) const ;
    int operator ==(const char     c) const ;
    int operator !=(const char     c) const ;
    friend int operator ==(const char *c, const String& s2) ;
    friend int operator !=(const char *c, const String& s2) ;
    friend int operator ==(const char  c, const String& s2) ;
    friend int operator !=(const char  c, const String& s2) ;

    // ���ڥ졼����>, <, >=, <=����ӱ黻��
    int operator > (const String& s2) const ;
    int operator < (const String& s2) const ;
    int operator >=(const String& s2) const ;
    int operator <=(const String& s2) const ;
    int operator > (const char    *c) const ;
    int operator < (const char    *c) const ;
    int operator >=(const char    *c) const ;
    int operator <=(const char    *c) const ;
    int operator > (const char     c) const ;
    int operator < (const char     c) const ;
    int operator >=(const char     c) const ;
    int operator <=(const char     c) const ;
    friend int operator > (const char *c, const String& s2) ;
    friend int operator < (const char *c, const String& s2) ;
    friend int operator >=(const char *c, const String& s2) ;
    friend int operator <=(const char *c, const String& s2) ;
    friend int operator > (const char  c, const String& s2) ;
    friend int operator < (const char  c, const String& s2) ;
    friend int operator >=(const char  c, const String& s2) ;
    friend int operator <=(const char  c, const String& s2) ;

    // ���ڥ졼����()�������黻��
    String& operator ()(const String& s) ;
    String& operator ()(const char   *c) ;
    String& operator ()(const char    c) ;

    // ���ڥ졼����+, +=��
    String operator +(const String& s) const ;
    String operator +(const char   *s) const ;
    String operator +(const char    c) const ;
    friend String operator +(const char *d, const String& s) ;
    friend String operator +(const char  d, const String& s) ;
    String& operator +=(const String& s) ;
    String& operator +=(const char   *s) ;
    String& operator +=(const char    c) ;

    // ���ڥ졼����*, *=��
    String operator *(int n) ;
    String& operator *=(int n) ;

    // �����ϥ��ڥ졼��
    friend std::ostream& operator <<(std::ostream& os, const String& s) ;
    friend std::istream& operator >>(std::istream& is, String& s) ;


    // ��ĥ

    // ʸ������ڤ�ͤ�ʱ�¦�������ޤ��ϲ��Ԥޤ���TAB��ʤ���
    String& Cut() ;
    friend String& Cut(String      s) ;
    friend String& Cut(const char *s) ;
} ;

// �������� String ��
String IntToString(int i) ;


#ifdef OVERRIDE_NEW_OPERATOR
void MemDebugFinal() ;
#endif	// #ifdef OVERRIDE_NEW_OPERATOR


#endif
