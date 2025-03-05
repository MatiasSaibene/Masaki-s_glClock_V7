
#include "MString.H"

String Left(const String &, int);
String Mid(String, int, int);
String Right(const String &, int);

// ���󥹥ȥ饯��
String::String(int n)
{
	length = n ;

	if (n == 0)
	{
		str = NULL ;
		Clear() ;
	}
	else
	{
		allocated = shouldAlloc(length) ;
		str = new char[allocated] ;
		memset(str, ' ', length) ;
		str[length] = '\0' ;
	}
}

String::String(const char *c)
{
//      cout << "String::String from const char * OK!!!!!!!" << endl ;

    length = strlen(c) ;
	if (length == 0)
	{
		str = NULL ;
		Clear() ;
		return ;
	}

/*
	if (length < MIN_ALLOC_UNIT)
		allocated = MIN_ALLOC_UNIT ;
	else
		allocated = (length + 1) * 2 ;
*/
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

	// �ǽ餫��̥뤬ͭ������Τǡ��̥�������ɲ�
    memcpy(str, c, length) ;
    str[length] = '\0' ; // �Ǹ�˥̥�ʸ���ղ�
}

String::String(const char *c, int n)
{
//      cout << "String::String from const *char, int OK!!!!!!!" << endl ;

	int slen = strlen(c) ;

    length = n ;
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

    if (slen < length)
    {
        // ���ԡ���������û�����Ϥ��ޤ����������
        memcpy(str, c, slen) ;
		memset(&(str[slen]) , ' ', length - slen) ;
    }
    else
    {
        // ���ԡ���������Ĺ����Ʊ������ñ���ɬ��ʬ���ԡ�
        memcpy(str, c, length) ;
    }

	// �Ǹ�˥̥�ʸ���ɲ�
	str[length] = '\0' ;
}

String::String(const String& s)
{
//    cout << "String::String from String OK!!!!!!!" << endl ;
//	if (this == &s) return ;

    length = s.length ;
	allocated = s.allocated ;

    str = new char[allocated] ;
    memcpy(str, s.str, length) ;
    str[length] = '\0' ; // �Ǹ�˥̥�ʸ���ղ�

//    *this = String(s.str, s.length) ;
}

String::String(const String& s, int n)
{
//    cout << "String::String from String, int  OK!!!!!!!" << endl ;
    length = n ;
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

    if (s.length < length)
    {
        // ���ԡ���������û�����Ϥ��ޤ����������
        memcpy(str, s.str, s.length) ;
		memset(&(str[s.length]), ' ', length - s.length) ;
    }
    else
    {
        // ���ԡ���������Ĺ����Ʊ������ñ���ɬ��ʬ���ԡ�
        memcpy(str, s.str, length) ;
    }

	// �Ǹ�˥̥�ʸ���ɲ�
	str[length] = '\0' ;

//    *this = String(s.str, n) ;
}

String::String(char c)
{
    str = NULL ;
    if (c == '\0')
	{
		str = NULL ;
		Clear() ;
	}
    else
    {
		length = 1 ;
		allocated = MIN_ALLOC_UNIT ;
		str = new char[allocated] ;
		str[0] = c ; str[1] = '\0' ;
    }
}


// ʸ�����Ĺ�����Ѳ���;�ä���ʬ�� ' '��
String& String::Length(int n)
{
	if (length == n)
		return *this ;
	else if (length > n)
	{
		// û���ʤä��Ȥ�
		Realloc(n) ;
		length = n ;
		return *this ;
	}
	else
	{
		// Ĺ���ʤä��Ȥ��Τߡ�������ʬ�˶���򥻥å�
		Realloc(n) ;
		memset(&(str[length]), ' ', n - length) ;
//		str[n] = '\0' ;
		length = n ;
		return *this ;
	}

//    return (*this = String(str, n)) ;
}


// n ʸ���ܤΣ�ʸ��
char String::Character(int n) const
{
	if (n < 1 || n > length) return '\0' ;
	return str[n - 1] ;
}

// n ʸ���ܤ�ʸ�����ѹ�
String& String::SetAt(int n, char c)
{
	if (n > 0 && n <= length)
		str[n - 1] = c ;
	return *this ;
}


String& String::ReplaceString(char s, char d)
{
	for (int n = 1 ; n <= length ; n ++)
	{
		if (str[n - 1] == s)
			str[n - 1] = d ;
	}

	return *this ;
}

String& String::ReplaceString(const String& s, const String& d)
{
	if (this == &s || this == &d) return *this ;

	int pos = 1 ;
	while (TRUE)
	{
		int start = Search(s) ;

		// 2000.06.08
		if (start < pos || !start)
			break ;

		int rlen = length - (start + s.length - 1) ;
		String right ;
		if (rlen > 0)
			right = ::Right(*this, rlen) ;

		if (start == 1)
		{
			*this = d + right ;
		}
		else
		{
			Length(start - 1) ;
			*this += d + right ;
		}

		pos = start + d.Length() ;
	}

	return *this ;
}


// Lower, Upper
String& String::Lower()
{
	for (int i = 0 ; i < length ; i ++)
	{
		if (isupper(str[i]))
			str[i] = tolower(str[i]) ;
	}

	return *this ;
}

String& String::Upper()
{
	for (int i = 0 ; i < length ; i ++)
	{
		if (islower(str[i]))
			str[i] = toupper(str[i]) ;
	}

	return *this ;
}


// �ե����ޥå�
String& String::Format(const char* format, ...)
{
	char buf[MAX_LINE_BUF] ;

	va_list args ;
	va_start(args, format) ;
//	int r = vsprintf(buf, format, args) ;
	vsprintf(buf, format, args) ;
	va_end(args) ;

	// ʸ���󥻥å�
	*this = buf ;
	return *this ;
}

// �ե����ޥå�
String& String::AddFormat(const char* format, ...)
{
	char buf[MAX_LINE_BUF] ;

	va_list args ;
	va_start(args, format) ;
//	int r = vsprintf(buf, format, args) ;
	vsprintf(buf, format, args) ;
	va_end(args) ;

	// ʸ���󥻥å�
	*this += buf ;
	return *this ;
}


// �ץ饤�١���
String& String::ReadAllLines(FILE *fp)
{
	while (TRUE)
	{
		char buf[MAX_LINE_BUF] ;
		char *s ;

		s = fgets(buf, MAX_LINE_BUF, fp) ;
		if (!s)
			break ;

		*this += s ;
	}
	return *this ;
}


// �ƥ����ȥե������ɤ߹���
String& String::ReadFile(const String& fileName)
{
	FILE *fp ;
	fp = fopen(fileName, "r") ;

	if (!fp)
		*this = "" ;
	else
	{
		*this = ReadAllLines(fp) ;
		fclose(fp) ;
	}

	return *this ;
}


// �ȡ��������
// �֤��줿�ȡ�����ʺǽ��ʸ����ˤϺ�������
// String("") ���֤����ޤǷ����֤�����
String String::Token(const String& delimiters, const String& comment)
{
	String	token ;

	// ���Υȡ�����Υ������Ȱ��֤򸡺�
	int start ;
	for (start = 1 ; start <= Length() ; start ++)
	{
		// �����ȹԥ����å�
		if (::Mid(*this, start, comment.Length()) == comment)
		{
			// �����Ȥʤ�ԤκǸ�ޤ����Ф�
			String right = ::Mid(*this, start, Length()) ;
			int next = right.Search('\n') ;

			if (next)
				start = next ;
			else	// ���Ԥ��ʤ��Ȥ������Ȥ�ʸ����κǸ�
				start = Length() ;

			continue ;
		}

		int flag = TRUE ;
		for (int c = 1 ; c <= delimiters.Length() ; c ++)
		{
			if (Character(start) == delimiters.Character(c))
			{
				flag = FALSE ;
				break ;
			}
		}

		// �ǥ�ߥ��Τɤ�Ȥ�ޥå����ʤ����Ϸ���
		if (flag)
			break ;
	}

	Mid(start, Length()) ;

	// ���Υȡ�����ν����ΰ��֤򸡺�
	int length ;
	for (length = 1 ; length <= Length() ; length ++)
	{
		// �����ȥ����å�������ǥ����ȤˤʤäƤ�����ϥ����Ȥ�ľ���ޤǡ�
		if (::Mid(*this, length, comment.Length()) == comment)
			break ;

		// �ǥ�ߥ��Τɤ줫���ĤǤ�ޥå�����Ф���ľ���ޤ�
		int flag = FALSE ;
		for (int c = 1 ; c <= delimiters.Length() ; c ++)
		{
			if (Character(length) == delimiters.Character(c))
			{
				flag = TRUE ;
				break ;
			}
		}

		if (flag)
			break ;
	}

	length -- ;
	token = ::Left(*this, length) ;
	Mid(length + 1, Length()) ;

	return token ;
}


// m ʸ���ܤ��� n ʸ��
String& String::Mid(int m, int n)
{
	// ���ԡ���Ĺ���Ȱ��֤�Ŭ����
    if (m < 1) m = 1 ;
    if (m > length || n < 0) return (this->Clear()) ;
	if (m + n > length + 1) n = length + 1 - m ;

	// ��ʣ�����ǽ�������뤿��
    memmove(str, &(str[m - 1]), n) ;

	// ̵�̤�����к��
	Realloc(n) ;
	length = n ;
	str[length] = '\0' ;

	return *this ;

/*
    if (m < 1) m = 1 ;
    if (m > length || n < 0) return (this->Clear()) ;
    if (m + n > length + 1) n = length + 1 - m ;

    String  w(n) ;
//    for (int i = 0 ; i < n ; i ++) w.str[i] = this->str[m + i - 1] ;
    memcpy(w.str, &(this->str[m - 1]), n) ;

    *this = w ;
    return *this ;
*/
}
String Mid(String s, int m, int n)
{
    return (s.Mid(m, n)) ;
}

// ������ n ʸ��
String& String::Left(int n)
{
    return (this->Mid(1, n)) ;
}
String Left(const String& s, int n)
{
    return (Mid(s, 1, n)) ;
}

// Método miembro
String& String::Right(int n) {
    if (n > length) n = length;
    this->Mid(length + 1 - n, n); // Supone que Mid está implementado
    return *this;
}

// Función global
String Right(const String& s, int n) {
    String w = s;
    return w.Right(n); // Llama al método miembro
}

// ���ꤵ�줿�ե�����̾�Υǥ��쥯�ȥ���ʬ���֤�
// �����鸡������"/" �⤷���� "\" �μ����ޤǤ��֤�
String String::FileDirectory() const
{
#ifdef WIN32
	char dpath = '\\' ;
#else
	char dpath = '/' ;
#endif

	String dir ;

	int locate = length - 1 ;
	while (locate > 0 && str[locate - 1] != dpath) locate -- ;
	if (locate <= 0)
		dir = "" ;
	else
		dir = ::Left(*this, locate - 1) ;

	return dir ;
}

// ���ꤵ�줿�ե�����̾�Υե�����͡�����ʬ���֤�
// �����鸡������"/" �⤷���� "\" �θ�Τߤ��֤�
String String::FileNamePart() const
{
#ifdef WIN32
	char dpath = '\\' ;
#else
	char dpath = '/' ;
#endif

	String dir ;

	int locate = length - 1 ;
	while (locate > 0 && str[locate - 1] != dpath) locate -- ;
	if (locate <= 0)
		dir = str ;
	else
		dir = ::Right(*this, length - locate) ;

	return dir ;
}

// ʸ���󥵡����ʺǽ�˰��פ��������֤���
int String::Search(const String& key) const
{
    if ((!key.length) || (!length)) return FALSE ; // ʸ����Ĺ�� 0 �ʤ� FALSE

    // ʸ�����Ĺ�����饭����������Ȥ����ޤǥ�����
    for (int i = 0 ; i < length - (key.length - 1) ; i ++)
    {
		char tmp = key.str[0] ;
		if (tmp == str[i])
		{
		    // ��������ʸ���ʤ餳�λ����ǳ���
		    if (key.length == 1)
				return (i + 1) ;
		    // ��������ʸ���ʾ�ξ��Ϥ���˥�����
		    else
		    {
			int j ;
			for (j = 1 ; j < key.length ; j ++)
			    if (str[i + j] != key.str[j]) break ;

			// ������ʸ�������������֤��Ƥ���г���
			if (j == key.length)
			    return (i + 1) ;
			}
		}
	}

    return FALSE ;
}
int String::Search(const char *key) const
{
    return Search(String(key)) ;
}
int String::Search(const char key) const
{
    return Search(String(key)) ;
}


// ���ڥ졼����/�ˤĤޤ�ϡ�������ʸ����򥵡����������פ���������֤�
int String::operator /(const String& key) const
{
    if ((!key.length) || (!length)) return FALSE ; // ʸ����Ĺ�� 0 �ʤ� FALSE

    int     n = 0 ;

    // ʸ�����Ĺ�����饭����������Ȥ����ޤǥ�����
    for (int i = 0 ; i < length - (key.length - 1) ; i ++)
    {
		char tmp = key.str[0] ;
		if (tmp == str[i])
		{
		    // ��������ʸ���ʤ餳�λ����ǳ���
		    if (key.length == 1)
				n ++ ;
			// ��������ʸ���ʾ�ξ��Ϥ���˥�����
		    else
		    {
				int j ;
				for (j = 1 ; j < key.length ; j ++)
				    if (str[i + j] != key.str[j]) break ;

				// ������ʸ�������������֤��Ƥ���г���
				if (j == key.length)
			    n ++ ;
		    }
		}
    }

    return n ;
}
int String::operator /(const char *key) const
{
    return *this / String(key) ;
}
int String::operator /(const char key) const
{
    return *this / String(key) ;
}

// �������� String ��
String IntToString(int i)
{
    String  s ;
    if (i < 0)
    {
		s = '-' ;
		i = -i ;
    }

    for (int flg = OFF, d = MAX_DIV_DEF ; d > 1 ; d /= 10)
    {
		if (d > i)
		{
		    if (flg) s += '0' ;
		    continue ;
		}
		flg = ON ;
		s += char(i / d + '0') ;
		i %= d ;
    }
    return (s + char(i + '0')) ;
}

// ���ڥ졼����=�������黻��
String& String::operator =(const String& s)
{
/*
    delete [] str ;
    length = s.length ;
    str = new char[length + 1] ;


//    for (int i = 0 ; i < length ; i ++) str[i] = s.str[i] ;
//    str[length] = '\0' ; // �Ǹ�˥̥�ʸ���ղ�

    memcpy(str, s.str, length + 1) ;
    return *this ;
*/
	if (this == &s) return *this ;

	Copy(s) ;
	return *this ;
}
String& String::operator =(const char *c)
{
	int clen = strlen(c) ;
	Realloc(clen) ;
	length = clen ;
	memcpy(str, c, length + 1);
	return *this ;

//    return (*this = String(c)) ;
}
String& String::operator =(const char c)
{
	Realloc(1, FALSE) ;
	length = 1 ;
	str[0] = c ;
	str[1] = '\0' ;
	return *this ;

//	return (*this = String(c)) ;
}


// ʸ����ӡʺǽ�� n ʸ���� s ��Ʊ���ǡ�n ���夬 s ��Ʊ�������ڤ���
// String str ;
// str.Equ("abcde",3) �Τ褦�� ("abcde", 3) ����Ӥ���ȡ�
// str = "abc", str = "abcd", str = "abcde" �� TRUE
// str �� "abcde"��5 ʸ���ˤ��Ĺ������ FALSE
// n <= 0 �ޤ��� ��ά����ȡ�n = 1 ��Ʊ��
int String::Equ(const String s, int n)
{
    if (n <= 0) n = 1 ;

    if (length > s.length || length < n) return FALSE ;
    String w = s ;
    w.Length(length) ;
    return (*this == w) ;
}

// n ʸ���羮��ӡ�strncmp()��Ʊ��������n ���ά����ȡ�strcmp() ��Ʊ����
int String::Cmp(const String s, int n)
{
    if (n < 0) return strcmp(str, s.str) ;
    else       return strncmp(str, s.str, n) ;
}

/*
// ���ڥ졼����==, !=����ӱ黻��
int operator ==(const String& s1, const String& s2)
{
    if (!strcmp(s1.str, s2.str)) return TRUE ;
    else                         return FALSE ;
}
int operator ==(const char *c, const String& s2)
{
    if (!strcmp(c, s2.str)) return TRUE ;
    else                    return FALSE ;
}
int operator !=(const String& s1, const String& s2)
{
    return !(s1 == s2) ;
}
int operator !=(const char *c, const String& s2)
{
    return !(c == s2) ;
}
*/


// ���ڥ졼����==, !=����ӱ黻��

// String Ʊ�Τ����
int String::operator ==(const String& s2) const
{
    if (!strcmp(str, s2.str)) return TRUE ;
    else                      return FALSE ;
}
int String::operator !=(const String& s2) const
{
    if (strcmp(str, s2.str)) return TRUE ;
    else                     return FALSE ;
//	{
//    if (strcmp(str, s2.str)) return TRUE ;
//    else                     return FALSE ;
//	}
}

// char * �Ȥ����
int String::operator ==(const char *c) const
{
    if (!strcmp(str, c)) return TRUE ;
    else                 return FALSE ;
}
int String::operator !=(const char *c) const
{
    if (strcmp(str, c)) return TRUE ;
    else                return FALSE ;
}

// char �Ȥ����
int String::operator ==(const char c) const
{
    if (length == 1 && str[0] == c) return TRUE ;
    else                            return FALSE ;
}
int String::operator !=(const char c) const
{
    return !(*this == c) ;
}

// ���դ� char * �Ȥ����
int operator ==(const char *c, const String& s2)
{
    if (!strcmp(c, s2.str)) return TRUE ;
    else                    return FALSE ;
}
int operator !=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str)) return TRUE ;
    else                   return FALSE ;
}

// ���դ� char �Ȥ����
int operator ==(const char c, const String& s2)
{
    return (s2 == c) ;
}
int operator !=(const char c, const String& s2)
{
    return (s2 != c) ;
}


/*
// ���ڥ졼����>, <, >=, <=���羮��ӱ黻��
int operator >(const String& s1, const String& s2)
{
    if (strcmp(s1.str, s2.str) > 0) return TRUE ;
    else                            return FALSE ;
}
int operator >(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) > 0) return TRUE ;
    else                       return FALSE ;
}
int operator <(const String& s1, const String& s2)
{
    if (strcmp(s1.str, s2.str) < 0) return TRUE ;
    else                            return FALSE ;
}
int operator <(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) < 0) return TRUE ;
    else                            return FALSE ;
}
int operator >=(const String& s1, const String& s2)
{
    return (s1 > s2 || s1 == s2) ;
}
int operator >=(const char *c, const String& s2)
{
    return (c > s2 || c == s2) ;
}
int operator <=(const String& s1, const String& s2)
{
    return (s1 < s2 || s1 == s2) ;
}
int operator <=(const char *c, const String& s2)
{
    return (c < s2 || c == s2) ;
}
*/

// ���ڥ졼����>, <, >=, <=���羮��ӱ黻��

// String Ʊ�Τ����
int String::operator >(const String& s2) const
{
    if (strcmp(str, s2.str) > 0) return TRUE ;
    else                         return FALSE ;
}
int String::operator <(const String& s2) const
{
    if (strcmp(str, s2.str) < 0) return TRUE ;
    else                         return FALSE ;
}
int String::operator >=(const String& s2) const
{
    if (strcmp(str, s2.str) >= 0) return TRUE ;
    else                          return FALSE ;
}
int String::operator <=(const String& s2) const
{
    if (strcmp(str, s2.str) <= 0) return TRUE ;
    else                          return FALSE ;
}

// char * �Ȥ����
int String::operator >(const char *c) const
{
    if (strcmp(str, c) > 0) return TRUE ;
    else                    return FALSE ;
}
int String::operator <(const char *c) const
{
    if (strcmp(str, c) < 0) return TRUE ;
    else                    return FALSE ;
}
int String::operator >=(const char * c) const
{
    if (strcmp(str, c) >= 0) return TRUE ;
    else                     return FALSE ;
}
int String::operator <=(const char *c) const
{
    if (strcmp(str, c) <= 0) return TRUE ;
    else                     return FALSE ;
}

// char �Ȥ����
int String::operator >(const char c) const
{
    return (*this > String(c)) ;
}
int String::operator <(const char c) const
{
    return (*this < String(c)) ;
}
int String::operator >=(const char c) const
{
    return (*this >= String(c)) ;
}
int String::operator <=(const char c) const
{
    return (*this <= String(c)) ;
}

// ���դ� char* �Ȥ����
int operator >(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) > 0) return TRUE ;
    else                       return FALSE ;
}
int operator <(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) < 0) return TRUE ;
    else                       return FALSE ;
}
int operator >=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) >= 0) return TRUE ;
    else                        return FALSE ;
}
int operator <=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) <= 0) return TRUE ;
    else                        return FALSE ;
}

// ���դ� char �Ȥ����
int operator >(const char c, const String& s2)
{
    return (String(c) > s2) ;
}
int operator <(const char c, const String& s2)
{
    return (String(c) < s2) ;
}
int operator >=(const char c, const String& s2)
{
    return (String(c) >= s2) ;
}
int operator <=(const char c, const String& s2)
{
    return (String(c) <= s2) ;
}


// ���ڥ졼����()�������黻��
String& String::operator ()(const String& s)
{
    return (*this = s) ;
}
String& String::operator ()(const char *c)
{
    return (*this = c) ;
}
String& String::operator ()(const char c)
{
    return (*this = c) ;
}


/*
// ���ڥ졼����+, +=��
String operator +(const String& d, const String& s)
{
    String  w(d.str, d.length + s.length) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + d.length] = s.str[i] ;
    return w ;
}
String operator +(char d, const String& s)
{
    String wd = String(d), w(wd, s.length + 1) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + 1] = s.str[i] ;
    return w ;
}
String operator +(const String& d, char s)
{
    String ws(s), w(d.str, d.length + 1) ;
    for (int i = 0 ; i < ws.length ; i ++) w.str[i + d.length] = ws.str[i] ;
    return w ;
}
String& String::operator +=(const char c)
{
    return (*this = *this + c) ;
}
String& String::operator +=(const String& s)
{
    return (*this = *this + s) ;
}
*/

// ���ڥ졼����+, +=��

// String Ʊ�Τ�Ϣ��
String String::operator +(const String& s) const
{
    String  w(str, length + s.length) ;

//    for (int i = 0 ; i < s.length ; i ++) w.str[i + length] = s.str[i] ;
    memcpy(&(w.str[length]), s.str, s.length) ;

    return w ;
}
// ���դ� char * �Ȥ�Ϣ��
String String::operator +(const char *s) const
{
    int slen = strlen(s) ;
    String  w(str, length + slen) ;

//    for (int i = 0 ; i < slen ; i ++) w.str[i + length] = s[i] ;
    memcpy(&(w.str[length]), s, slen) ;

    return w ;
}
// ���դ� char �Ȥ�Ϣ��
String String::operator +(const char s) const
{
    String w(str, length + 1) ;
    w.str[w.length - 1] = s ;
    return w ;
}

// ���դ� char * �Ȥ�Ϣ��
String operator +(const char *d, const String& s)
{
    int     dlen = strlen(d) ;
    String  w(d, dlen + s.length) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + dlen] = s.str[i] ;
    return w ;
}

// ���դ� char �Ȥ�Ϣ��
String operator +(const char d, const String& s)
{
    String wd(d), w(wd, s.length + 1) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + 1] = s.str[i] ;
    return w ;
}

// Ϣ������
String& String::operator +=(const String& s)
{
	if (this == &s)
	{
		*this += String(s) ;
		return *this ;
	}

	Realloc(length + s.length) ;
	memcpy(&(str[length]), s.str, s.length + 1) ;
	length += s.length ;

	return *this ;
/*
    int len = length ;
    Length(len + s.length) ;
    for (int i = 0 ; i < s.length ; i ++)
      str[i + len] = s.str[i] ;

    return (*this) ;
*/
}
String& String::operator +=(const char *s)
{
	int slen = strlen(s) ;
	Realloc(length + slen) ;
	memcpy(&(str[length]), s, slen + 1) ;
	length += slen ;

	return *this ;

//    return (*this = *this + s) ;
}
String& String::operator +=(const char c)
{
	Realloc(length + 1) ;
	length ++ ;
	str[length - 1] = c ;
	str[length] = '\0' ;

	return *this ;
/*
    Length(length + 1) ;
    str[length - 1] = c ;
    return (*this) ;
*/
}


// ���ڥ졼����*, *=��
String String::operator *(int n)
{
	String s(length * n) ;
	for (int i = 0 ; i < n ; i ++)
		memcpy(&(str[length * i]), str, length) ;
	length *= n ;
	return s ;
/*
    String  s ;
    for (int i = 0 ; i < n ; i ++) s += *this ;
    return s ;
*/
}
String& String::operator *=(int n)
{
	Realloc(length * n) ;
	for (int i = 1 ; i < n ; i ++)
		memcpy(&(str[length * i]), str, length) ;
	length *= n ;
	return *this ;

//    return (*this = *this * n) ;
}

// �����ϥ��ڥ졼��
std::ostream& operator <<(std::ostream& os, const String& s)
{
    os << s.str ;
    return os ;
}
std::istream& operator >>(std::istream& is, String& s)
{
    is.getline(s.str, MAX_DIV_DEF);
    return is ;
}


// ��ĥ

// ʸ������ڤ�ͤ�ʱ�¦�������ޤ��ϲ��Ԥޤ���TAB��ʤ���
String& String::Cut()
{
    int  i ;
    for (i = length ; i >= 1 ; i --)
    {
//	if (Character(i) != ' '  &&
//	    Character(i) != '\n' &&
//	    Character(i) != '\t') break ;
	if (!isspace(Character(i))) break ;
    }
    Length(i) ; // ʸ�����Ĺ�����ڤ�ͤ��

    return *this ;
}


// ʸ������ڤ�ͤ�ʱ�¦�������ޤ��ϲ��Ԥޤ���TAB��ʤ���
String& Cut(String s)
{
    return s.Cut() ;
}

// ʸ������ڤ�ͤ�ʱ�¦�������ޤ��ϲ��Ԥޤ���TAB��ʤ���
String& Cut(const char *s)
{
    String tmp(s) ;
    return tmp.Cut() ;
}






#ifdef OVERRIDE_NEW_OPERATOR


//��������������������������������������������������������������������������
//      ����Ρ��ɹ�¤��
//��������������������������������������������������������������������������
typedef struct memnode_tag
{
        struct memnode_tag      *pNext;
        unsigned long                        dwSize;
        int                                     pMem[1];
} MEMNODE;

// ��¤�ΤΥإå���ʬ�Υ�����

#define MEMNODE_HDRSIZE (sizeof(MEMNODE*) + sizeof(unsigned long))



//��������������������������������������������������������������������������
//      �ϥå���ơ��֥��Ϣ
//��������������������������������������������������������������������������
// �ϥå���ơ��֥���礭���ʣ����߾�Ǥ��뤳�Ȥ�ɬ�ܡ�
//#define HASHTABLE_SIZE  0x1000
#define HASHTABLE_SIZE  0x1000

// �ϥå���ơ��֥�
static MEMNODE memtbl[HASHTABLE_SIZE];

// �ݥ��󥿤���ϥå��奭������륤��饤��ؿ�
static inline unsigned long GetHashKey( void* p )
{
        return (int)p & (HASHTABLE_SIZE - 1);
}



//��������������������������������������������������������������������������
//      �ǡ�������
//��������������������������������������������������������������������������
// �����������׵����ʼ��Դޤ��
static int nAllocCtr = 0;

// �ե꡼�׵����ʼ��Դޤ��
static int nFreeCtr = 0;

// ���������ȼ��Բ��
static int nAllocFail = 0;

// �ե꡼���Բ��
static int nFreeFail = 0;

// NULL �ե꡼���
static int nFreeNull = 0;

// ���ߤΥ�������̡�ñ������¡�
static int nAllocAmt = 0;

// ������Υ��������
static int nAllocAmtMax = 0;

// ����Ū�����å���λ
static int isChecked = FALSE ;


// ̤��������ơ��֥�
#define MAX_UNFREE_TABLE 10000
static int nLeaks ;
static MEMNODE *unfree[MAX_UNFREE_TABLE] ;


//��������������������������������������������������������������������������
//      �����С��饤�ɤ���new�黻��
//��������������������������������������������������������������������������
void* :: operator new( size_t n )
{
        MEMNODE*        pNewNode;
        MEMNODE*        pNode;
        unsigned long           nKey;

		if (!nAllocCtr)
		{
			for (int i = 0 ; i < HASHTABLE_SIZE ; i ++)
				memtbl[i].pNext = NULL ;
		}

        nAllocCtr++;

        // �Ȥꤢ�����ΰ�����
        pNewNode = (MEMNODE*)malloc( n + sizeof(MEMNODE) );
        if ( pNewNode == NULL )
        {
                nAllocFail++;
                return NULL;
        }


/*
		else if (//((int)pNewNode) == (0xD82B40) ||
				 //((int)pNewNode) == (0xD82DE0) ||
//				 ((int)pNewNode) == (0xD83830) ||
				 ((int)pNewNode) == (0xD837B0))
		{
			pNewNode = pNewNode ;
//			inline int logprintf(const char* format, ...) ;
//			logprintf("pNewNode:%lX\n", pNewNode) ;
		}

//			inline int logprintf(const char* format, ...) ;
//			logprintf("pNewNode:%lX  pNewNode->pMem:%lX\n", (int)pNewNode, (int)pNewNode->pMem) ;
*/

        pNewNode->pNext = NULL;
        pNewNode->dwSize = n;

        // ��������̤ν���
        nAllocAmt += n;
        if ( nAllocAmt > nAllocAmtMax ) nAllocAmtMax = nAllocAmt;

        // �ϥå���ơ��֥����Ƭ�Υ���ȥ������
        nKey    = GetHashKey(pNewNode);
        pNode   = &memtbl[nKey];

        // Ϣ��κǸ����򤿤���
        while ( pNode->pNext != NULL )
        {
                pNode = pNode->pNext;
        }

        // Ϣ��κǸ����� pNewNode ���ɲ�
        pNode->pNext = pNewNode;

        // pMem�����ݤ��줿�ΰ�Ȥʤ�
        return (void*)pNewNode->pMem;
}



//��������������������������������������������������������������������������
//      �����С��饤�ɤ���delete�黻��
//��������������������������������������������������������������������������
void :: operator delete( void* p )
{
        MEMNODE*        pDelNode;
        MEMNODE*        pNode;
        int                     nKey;

        nFreeCtr++;

		if (!p)
		{
			nFreeFail ++ ;
			nFreeNull ++ ;
			return ;
		}
        
        // �ݥ���p���Ρ��ɤΥݥ��󥿤򻻽Ф���
        pDelNode = (MEMNODE*)((int)p - MEMNODE_HDRSIZE);

		if (isChecked)
		{
			for (int i = 0 ; i < nLeaks ; i ++)
			{
				if (unfree[i] == pDelNode)
				{
					unfree[i] = NULL ;
					free(pDelNode) ;
				}
			}

			FILE *fp = fopen("leakcheck.log", "w") ;
			if (fp)
			{
				for (int i = 0 ; i < nLeaks ; i ++)
				{
					if (unfree[i])
						fprintf(fp, "leak: address: 0x%lX\n", (int)unfree[i]) ;
				}
//				fprintf(fp, "already checked!  address: %lX  delete request: %ld\n", ((int)p - MEMNODE_HDRSIZE), nFreeCtr) ;
				fprintf(fp, "delete request: %ld\n", nFreeCtr) ;
				fclose(fp) ;
			}
			return ;
		}

		if ((int)pDelNode == 0x00d729d0)
		{
			pDelNode = pDelNode ;
		}

        // pDelNode���ϥå��奭�������
        nKey = GetHashKey(pDelNode);
        
        // �ϥå���ơ��֥��ꡤpDelNode�ΰ�����ΥΡ��ɤ򤿤���
        pNode = &memtbl[nKey];
        while ( pNode->pNext != pDelNode && pNode->pNext != NULL )
        {
                pNode = pNode->pNext;
        }

        // �Ρ��ɤ����Ĥ���ʤ��ä����ν����ʥ��顼��
        if ( pNode->pNext == NULL )
        {
                nFreeFail++;
                return;
        }

        // Ϣ��ηҤ�ľ��
        pNode->pNext = pDelNode->pNext;

        // �ΰ�γ���
        nAllocAmt -= pDelNode->dwSize;
        free(pDelNode);
}



//��������������������������������������������������������������������������
//      ���׽���
//��������������������������������������������������������������������������
void MemDebugFinal()
{
        char    pStrBuf[256];
        int             i;
		int     leakBytes = 0 ;

		isChecked = TRUE ;
		for (i = 0 ; i < MAX_UNFREE_TABLE ; i ++)
			unfree[i] = NULL ;


//		inline int logprintf(const char* format, ...) ;

		nLeaks = 0 ;

        // �ϥå���ơ��֥����������̤�������ΰ褬���ä���ٹ��ɽ����
        for ( i=0 ; i<HASHTABLE_SIZE ; i++ )
        {
                MEMNODE* pNode = memtbl[i].pNext;

                while ( pNode )
                {
                        MEMNODE* pNext = pNode->pNext;
//                        sprintf( pStrBuf, ">> Memory leak found!!  key: %d  address:%lX  pNext:%lX  size:%d bytes\r\n", i, (int)pNode, (int)pNode->pNext, (int)pNode->dwSize) ;
                        sprintf( pStrBuf, ">> Memory leak found!!  address:%lX  %d bytes\r\n", (int)pNode, (int)pNode->dwSize) ;
//                        logprintf( pStrBuf );
						leakBytes += pNode->dwSize ;
//                        free( pNode );

						unfree[nLeaks] = pNode ;
						nLeaks ++ ;

                        pNode = pNext;
                }
        }

/*
        // ���׷�̤����
        sprintf( pStrBuf, ">> new    request : %ld times\r\n", nAllocCtr );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete request : %ld times\r\n", nFreeCtr );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> new    failure : %ld times\r\n", nAllocFail );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete failure : %ld times\r\n", nFreeFail );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete NULL    : %ld times\r\n", nFreeNull );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> allocation max : %ld bytes\r\n", nAllocAmtMax );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> total %d bytes leak\r\n", leakBytes );
        logprintf( pStrBuf );
*/
}


#endif	// #ifdef OVERRIDE_NEW_OPERATOR
