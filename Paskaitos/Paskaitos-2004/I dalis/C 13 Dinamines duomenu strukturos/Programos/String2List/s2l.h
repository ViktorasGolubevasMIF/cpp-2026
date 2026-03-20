typedef	char Data;
struct	LinkedList {
	Data data;
	struct LinkedList *next;
};
typedef	struct LinkedList Element;
typedef	Element	*Link;
typedef char *String;

Link s2l_iter (String);
Link s2l_recur (String);
void l2prn_iter (Link);
void l2prn_recur (Link);
