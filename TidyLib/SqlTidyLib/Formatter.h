#ifndef __SQLFORMATTER_H
#define __SQLFORMATTER_H

class SQLFormatter
{
public:
	// Constructor
	SQLFormatter(int n_maxLookahead = 2, int n_indent = 2, bool n_endLineOnSubQuery = true, int n_minSubqueryLength = 100, char n_indentationChar = ' ');
	// Destructor
	~SQLFormatter();

protected:
	bool _isSqlToken(const char *start, const char *end);
	void _output(const char *start, const char *end, bool endLine);
	void _formatSql(char *start, char *end, char *dest, int indent, bool lastWasToken);
	void _addIndentation(char *&p, int indent);
	void _addIndentation(char *&p, int indent, char indentationChar);
	void _moveWhileSpaces(char *&p);
	void _moveUntil(char *&p1, char *p2, char *&res);
	void _moveOneWord(char *&p1, char *&end, char *&res);
	void _handleMaxLookaheadReached(char *&p1, char *&p2, char *&res, int indent, bool &lastWasToken, int &lookahead);
	void _handleTokenOrEndline(char *&p1, char *&p2, char *&res, int &indent, bool &lastWasToken, int &lookahead, bool isFirstWord);
	void _handleSubQuery(char *&p1, char *&p2, char *&res, int indent, bool &lastWasToken, int &lookahead);
	void _handleIncreaseLookahead(char *&p1, char *&p2, char *&res, int indent, bool &lastWasToken, int &lookahead);

public :
	// Formats given sql string. Splits short subqueries.格式化SQL语句，分割短嵌套子查询
	void formatSql(char *sqlString, char *dest);
	// Flattens given sql string.弄平
	void flattenSql(char *sqlString, char *dest);
	// Checks if given strings have same length (not counting endline and tabs)比较字符串长度
	bool sameLength(char *str1, char *str2);

private :
	int  _maxLookahead;
	int  _indent;
	int  _minSubqueryLength;
	bool _endLineOnSubQuery;
	char _indentationChar;
};
#endif // __SQLFORMATTER_H
