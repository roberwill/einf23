#include "string.h"

int TYPE;

StrObject build_str  () {

	StrObject str;

	str.split = str_split;
	str.join = str_join;
	str.slice = str_slice;
	str.is_digit = str_is_digit;
	str.to_int = str_to_int;
	str.to_float = str_to_float;
	str.include = str_include;
	str.is_of_type = str_is_of_type;

	return str;

}

/*
* @Description
* It splits a string into tokens according to a certain char
* @Return
* The StringArray containing the tokens
* @Var
* str: the string to be tokenized
* dlm: the char to be used as tokenize-criterium
* nt: the number o tokens
*/


StringArray str_split(char dlm, int* nt, String str) {
	
	StringArray R;
	int len  = (int)strlen(str);
	int i, k, j, r_count, tokens;
	int n_chars, next = 0;

	//counts the number of tokens
	for (tokens = i = 0; i < len; i++)
		if (str[i] == dlm)
			tokens++;

	tokens++;
	//the number of tokens is assigned to "nt"
	*nt = tokens;

	//allocates the StringArray
	R = (String*) calloc(tokens, sizeof(String));

	//tokenizes the string
	for (i = 0; i < tokens; ++i) {
		
		//counts the number of chars of the "i" token
		for (n_chars = 0, k = next; ; k++) {
			//loop stops when it find the "dlm" or the end of the String
			if (str[k] == dlm || k == len) break;
			n_chars++; 
		}

		//allocates memory for the "i" token. 
		//the "+ 1" is due to '\0'
		R[i] = (String) calloc(n_chars + 1, sizeof(char));

		//copies the "i" token
		//r_count: controls the position of each char of the "i" token
		//next: is where the "first char" of the "i" token is to be found
		for (j = next, r_count = 0; r_count < n_chars; j++, r_count++) {
			R[i][r_count] = str[j];
		}

		//null-terminates the String
		R[i][r_count] = '\0';
		next = k + 1;

	}

	//return the StringArray
	return R;

}


/*
* @Description
* Joins n strings using "dlm" as separator
* @Return
* The string containg all strings + dlm
* @Var
* dlm: the separator/delimiter
* n: the number of strings to concatenate
* ... : the strings to be joined
*/

String str_join (char dlm, int n, ...) {

	int i, size = 0, s_size;
	String R, temp;
	char c[2];
	va_list str; //declare variadic function list
	
	//copies dlm to a local String
	c[0] = dlm;
	c[1] = '\0';
	
	//initialize variadic function list
	//it must be the param right before "..."
	va_start(str, n); 	
	
	//allocates space for R.. Here, no "space" at all
	R = (String) calloc(size, sizeof(char));

	//concatenates all string into one using "dlm"
	for (i = 0; i < n; i++) {

		//shift an argument from the list of type "char*"
		temp  =  va_arg(str, char*);
		//gets its size
		s_size = (int)strlen(temp);
		//computes the size that R needs.. "+1" is to store "dlm"
		size +=  s_size + 1;
		//reallocates space for R of zise "size"
		R = (String) realloc(R, size*sizeof(char));
		//joins the string	
		strcat(R, temp);
		//joins the "dlm" unless it is the last string
		if((i + 1) != n) strcat(R, c);
		
	}

	//terminate variadic function list
	va_end(str); 
	//return the string containing all of the strings + dlm
	return R;

}

/*
* @Description
* checks whether a string is a digit or not
* @Return
* TRUE or FALSE
* @Var
* str: string to be analised
*/

int str_is_digit(String str) {
	
	int len = (int)strlen(str);
	int i, fl = 0, start = 0;


	if (len == 1 && str[0] == '.')
		return FALSE;

    if (len == 1 && str[0] == '-')
        return FALSE;
    
    if (str[0] == '-')
        start = 1;
    
	for (i = start; i < len; i++) {
		
		//to handle floating point numbers :D
		if (str[i] == '.') {
			
			if (fl == 1)
				return FALSE;
			else
				fl++;

			continue;
		} 

		if ( !isdigit(str[i]) )
			return FALSE;
		
	}

	if (fl == 1)
		TYPE = FLOAT;
	else
		TYPE = INT;

	return TRUE;
}

/*
* @Description
* Converts a given string to an integer
* @Return
* true or false
* @Var
* str: the string to be converted
* num: wheere the converted int will have its value stored
*/

int str_to_int (String str, int* num) {

	if (str_is_digit(str)){
		*num = atoi(str);
		return TRUE;
	}

	else {
		*num = 0;
		return FALSE;
	}

}

/*
* @Description
* Converts a given string to a float
* @Return
* true or false
* @Var
* str: the string to be converted
* num: wheere the converted float will have its value stored
*/

int str_to_float (String str, int* num) {
	

	if (str_is_digit(str)){
		*num = atof(str);
		return TRUE;
	}

	else {
		*num = 0;
		return FALSE;
	}

}

/*
* @Description
* if the string is a number, it returns its type (float or int)
* it works alongside str_is_digit(..);
* @Return
* the "type"
*/

char str_is_of_type   () {
	return TYPE;
}

/*
* @Description
* checks whether a string is a substring of another
* @Return
* true or false
* @Var
* str: the string which possibly contains the substring
* substr: the substring
*/

int str_include (String str, String substr) {

	if ( strstr(str, substr) != NULL)
		return TRUE;
	else
		return FALSE;

}

/*
* @Description
* slices a piece of a certain string relying on a starting point as
* well as an ending point. It works with positive indexes, however, if
* it is given any negative index it is transformed into a positive one.
* "end" must be greater than "start", in the meantime, it is no problem at all
* @Return
* the slice of the string if it was given valid indexes and nil if not
* @Var
* str: the string which contains the "slice"
* start: from where to start slicing
* end: where to stop slicing
*/

String str_slice (String str, int start, int end) {

	String slice;
	
	int str_len = (int)strlen (str);
	int len, i, slice_counter;

	if (start < 0)
		start *= -1;

	if (end < 0)
		end *= -1;

	if (start > end) {
		int temp  = start;
			start = end;
			end   = temp;
	}

	if (start > str_len || end > str_len) 
		return nil;


	len   = (end - start) + 2;
	slice = (String)calloc(len, sizeof(char));
	
	start--;
	end--;
	
	for (i = start, slice_counter = 0; i <= end; i++)
	{
		slice[slice_counter] = str[i];
		slice_counter++;
	}

	slice[slice_counter] = END_STRING;

	return slice;

}













