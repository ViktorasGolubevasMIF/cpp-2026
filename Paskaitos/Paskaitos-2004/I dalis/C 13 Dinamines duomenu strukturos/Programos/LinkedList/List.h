typedef	char Data;
struct	LinkedList {
	Data data;
	struct LinkedList *next;
};
typedef	struct LinkedList Element, *Link;

void insert_element(Link*, Data);
void delete_element(Link*);
void insert_element_at_tail(Link, Data);
void delete_element_at_tail(Link);
Link next_element(Link);
Data get_data(Link);