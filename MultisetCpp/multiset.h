#ifndef MULTISET_H
#define MULTISET_H
#include <iostream>
#include <exception>
#include <assert.h>

/*
TODO
[V] - cavare i *prev
[V] - implementare operator<<
[V] - Elementi devono essere immutabili
[V] - commenti
[V] - constcorrectness

- eccezioni
- test vari in main.cpp
- rimuovere parti di codice commentate e commenti di debug
- aggiungere #IFNDEBUG etc..
- doc
- relazione
- test valgrind
*/


/**
	Classe Multiset.
	Classe che implementa un MultiSet di elementi generici T.
	Un MultiSet � come un insieme di dati che pu� contenere duplicati.
*/
template <typename T>
class multiset {
public:
	typedef unsigned int size_type; ///< Tipo custom per le size o quantit�.

private:
	/**
		Struct node - Struttura a nodi.
		Struct che rappresenta un nodo di una struttura dati a mo' di lista ovvero le cui peculiarit� sono la dimesnione variabile e la dinamicit�.
		I dati di riferimento sono l'elemento di tipo T e la quantit� in cui esso � presente le multiset.
	*/
	struct node {
		T object;			///< Elemento di tipo T presente nel multiset.
		size_type amount;	///< Quantit� in cui l'elemento � presente nel multiset.
		node *next;			///< Puntatore al nodo successivo a quello attuale (permette la realizzazione di una lista monodirezionale).

		/**
			Costruttore default.
		*/
		node() : amount(0), next(0) { }

		/**
			Costruttore Primario.
			Tutti i parametri del nodo vengono passati esplicitamente.
			@param obj Elemento di tipo T.
			@param amou Quantit� dell'elemento.
			@param n	Puntatore al nodo successivo.
		*/
		node(const T obj, size_type amou, node *n) : object(obj), amount(amou), next(n) { }

		/**
			Overload Costruttore Primario.
			Il nodo successivo non viene specificato (usato solitamente per creare la testa della struttura dati).
			@param obj Elemento di tipo T.
			@param amou Quantit� dell'Elemento.
		*/
		node(const T obj, size_type amou) : object(obj), amount(amou), next(0) { }

		/**
			Overload Costruttore Primario.
			Il nodo successivo non viene specificato (usato solitamente per creare la testa della struttura dati).
			La quantit� non specificata viene messa di default a 1.
			@param obj Elemento di tipo T.
		*/
		node(const T obj) : object(obj), amount(1), next(0) { }

		/**
			Overload Costruttore Primario.
			La quantit� non specificata viene messa di default a 1.
			@param obj Elemento di tipo T.
			@param n	Puntatore al nodo successivo.
		*/
		node(const T obj, node *n) : object(obj), amount(1), next(n) { }


		/**
			Copy constructor.
			@param other Nodo da copiare.
		*/
		node(const node &other) : object(other.object), amount(other.amount), next(other.next) { }

		/**
			Distruttore.
		*/
		~node() { }

		/**
			Operatore "=" di assegnamento.
			@param other Nodo da cui copiare i dati.
			@return this Puntatore al nodo attuale.
		*/
		node &operator=(const node &other) {
			if (this != &other) {
				object = other.object;
				amount = other.amount;
				next = other.next;
			}
			return *this;		///< Concatenzaione assegnamenti.
		}
	};

	node *head;					///< Puntatore che punta al primo nodo della struttura dati (==0 se la struttura � vuota).
	size_type objects_amount;	///< Quantit� di elementi totali nel Multiset come da specifica (vengono conteggiate anche le occorrenze multiple di un medesimo elemento).
	size_type nodes_amount;		///< Quantit� di nodi nella struttura dati (ovvero quantit� di elementi senza conteggiare occorrenze multiple di un medesimo elemento).

public:

	/**
		Costruttore di default.
	*/
	multiset(void) : head(0), objects_amount(0), nodes_amount(0) {
		#ifndef NDEBUG
		std::cout << "# multiset<T>::multiset(void)" << std::endl;
		#endif
	}

	/*try {

	} catch (bad_alloc &e) {
		std::cout << e.what() << std::endl;
		throw;
	} catch (...) {
		std::cout << e.what() << std::endl:
		throw;
	}*/

	/**
		Distruttore.
		Deallocazione della struttura dati di nodi utilizzando una funziona di appoggio clear().
	*/
	~multiset() {
		clear();
	}

private:

	/**
		Funzione di deallocazione struttura dati di nodi per utilizzo privato.
		Passo in rassegna nodo per nodo (seguendo la struttura dati partendo dalla testa) e lo elimino.
	*/
	void clear() {
		node *t1 = head;
		node *t2 = 0;

		while (t1 != 0) {
			t2 = t1->next;
			delete t1;
			t1 = t2;
		}
	}

public:

	/**
		Copy Constructor.
		@param other Multiset da copiare.
	*/
	multiset(const multiset &other) : head(0), objects_amount(0), nodes_amount(0) {
		node *tmp = other.head;
		try {
			while (tmp != 0) {
				add(tmp->object, tmp->amount);
				tmp = tmp->next;
			}
		} catch (...) {
			clear(); //(!) commento eccezione
			throw;
		}
	}

	/**
		Operatore Assegnamento.
		@param other Multiset da cui copiare i dati.
		@return this Puntatore al multiset attuale.
	*/
	multiset &operator=(const multiset &other) {
		if (this != &other) {
			multiset tmp(other);
			std::swap(this->head, tmp.head);
			std::swap(this->objects_amount, tmp.objects_amount);
			std::swap(this->nodes_amount, tmp.nodes_amount);
		}
		return *this; ///< Concatenzaione assegnamenti.
	}

	/**
	//(!)Operatore Assegnamento Templato.
	*/
	/*template<typename Q>
	multiset &operator=(const multiset<Q> &other) {
		multiset<T> ms;
		try {
			node *tmp = other.get_head();
			while (tmp != 0) {
				ms.add(static_cast<T>(tmp->object), tmp->amount);
				tmp->next;
			}
		} catch (...) {
			std::cout << "ERRORE: static_cast fallito." << std::endl;
			throw;
		}
		return *this;
	}*/

	/**
		Operazione add pubblica.
		Aggiunge un elemento al multiset.
		@param obj Elemento da aggiungere al multiset.
	*/
	void add(const T &obj) {
		add(obj, 1);
	}

private:

	/**
		Overload privato di add.
		Operazione di aggiunta di una quantit� positiva di elementi.
		@param obj Elemento da aggiungere al multiset.
		@n Quantit� da aggiungere dell'elemento obj.
	*/
	void add(const T &obj, size_type n) {
		node *t = head;
		bool found = false;
		while (t != 0) {
			if (t->object == obj) { //(!) Eccezione: Uguaglianza fra oggetti complessi non esiste
				//se l'elemento � gi� presente allora aumentane la quantit� di n
				found = true;
				t->amount += n;
				t = 0;
			} else {
				t = t->next;
			}
		}
		if (!found) {
			//altrimenti aggiungi nodo
			if (head == 0) {
				head = new node(obj); //(!) Eccezione su new
				head->amount = n;
			} else {
				node *tmp = head;
				head = new node(obj, tmp); //(!) Eccezione su new
				head->amount = n;
			}
			nodes_amount++;
		}
		objects_amount += n;
	}

public:

	/**
		Operazione remove.
		Rimuove un elemento dal multiset.
		Se sono presenti pi� elementi uguali ne rimuove uno (decrementa la quantit� di quell'elemento).
		@param obj Elemento da rimuovere.
		@return removed True se l'elemento � stato rimosso, False altrimenti.
	*/
	bool remove(const T &obj) {
		bool removed = false;
		node *curr = head;
		node *prev = 0;
		while (curr != 0) {
			if (curr->object == obj) { //(!) Eccezione uguaglianza
				if (curr->amount > 1) {
					curr->amount--;
				} else {
					node *next = curr->next;
					/*if (next != 0) {
						next->prev = p;
					}*/
					if (prev != 0) {
						prev->next = next;
					} else {
						head = next;
					}
					delete curr;
					nodes_amount--;
				}
				objects_amount--;
				removed = true;
				curr = 0;
			} else {
				prev = curr;
				curr = curr->next;
			}
		}
		return removed;
	}

	/**
		Operazione get_objects_amount.
		@return objects_amount Numero elementi del multiset (contando i duplicati).
	*/
	/*size_type get_objects_amount() {
		return objects_amount;
	}*/

	/**
		Operazione get_objects_amount costante.
		@return objects_amount Numero elementi del multiset (contando i duplicati).
	*/
	size_type get_objects_amount() const {
		return objects_amount;
	}

	/**
		Operazione get_nodes_amount.
		@return nodes_amount Numero nodi nel multiset (numero di elementi senza contare duplicati).
	*/
	/*size_type get_nodes_amount() {
		return nodes_amount;
	}*/

	/**
		Operazione get_nodes_amount costante.
		@return nodes_amount Numero nodi nel multiset (numero di elementi senza contare duplicati).
	*/
	size_type get_nodes_amount() const {
		return nodes_amount;
	}

	/**
		Operazione object_count.
		Conta il numero di occorrenze di un certo elemento nel multiset.
		@param obj Elemento di cui contare le occorrenze.
		@return count Numero di occorrenze.
	*/
	/*size_type object_count(T &obj) {
		node *tmp = head;
		size_type count = 0;
		while (tmp != 0) {
			if (obj == tmp->object) {
				count = tmp->amount;
				tmp = 0;
			} else {
				tmp = tmp->next;
			}
		}
		return count;
	}*/

	/**
		Operazione object_count costante.
		Conta il numero di occorrenze di un certo elemento nel multiset.
		@param obj Elemento di cui contare le occorrenze.
		@return count Numero di occorrenze.
	*/
	/*size_type object_count(T &obj) const {
		node *tmp = head;
		size_type count = 0;
		while (tmp != 0) {
			if (obj == tmp->object) { //(!) Eccezione uguaglianza
				count = tmp->amount;
				tmp = 0;
			} else {
				tmp = tmp->next;
			}
		}
		return count;
	}*/

	/**
		Operazione object_count.
		Conta il numero di occorrenze di un certo elemento nel multiset.
		@param obj Elemento di cui contare le occorrenze.
		@return count Numero di occorrenze.
	*/
	/*size_type object_count(const T &obj) {
		node *tmp = head;
		size_type count = 0;
		while (tmp != 0) {
			if (obj == tmp->object) {
				count = tmp->amount;
				tmp = 0;
			} else {
				tmp = tmp->next;
			}
		}
		return count;
	}*/

	/**
		Overload operazione object_count costante.
		Conta il numero di occorrenze di un certo elemento costante nel multiset.
		@param obj Elemento costante di cui contare le occorrenze.
		@return count Numero di occorrenze.
	*/
	size_type object_count(const T &obj) const {
		node *tmp = head;
		size_type count = 0;
		while (tmp != 0) {
			if (obj == tmp->object) { //(!) Eccezione uguaglianza
				count = tmp->amount;
				tmp = 0;
			} else {
				tmp = tmp->next;
			}
		}
		return count;
	}

	/**
		Operazione contains costante.
		@param obj Elemento di cui controllare la presenza nel multiset.
		@return bool True se l'elemento � presente, false altrimenti.
	*/
	bool contains(const T &obj) const {
		/*node *tmp = head;
		bool found = false;
		while (tmp != 0) {
			if (obj == tmp->object) {
				found = true;
				tmp = 0;
			} else {
				tmp = tmp->next;
			}
		}
		return found;*/
		return object_count(obj) > 0;
	}

	/**
	* Operatore ==.
	*
	*/
	/*bool &operator== (const multiset &other) {
		bool equals = true;
		if (objects_amount == other.get_objects_amount() && nodes_amount == other.get_nodes_amount()) {
			node *tmp = head;
			while (tmp != 0) {
				if (object_count(tmp->object) != other.object_count(tmp->object)) {
					equals = false;
					tmp = 0;
				} else {
					tmp = tmp->next;
				}
			}
		} else {
			equals = false;
		}
		return equals;
	}*/

	/**
		Operatore "==" di confronto (uguaglianza) costante.
		Confronta due multiset in base agli elementi da loro contenuti.
		@param other Multiset con cui confrontare this.
		@return equals True se i due multiset sono uguali, false altrimenti.
	*/
	bool operator== (const multiset &other) const {
		bool equals = true;
		if (objects_amount == other.get_objects_amount() && nodes_amount == other.get_nodes_amount()) {
			node *tmp = head;
			while (tmp != 0) {
				if (object_count(tmp->object) != other.object_count(tmp->object)) {
					equals = false;
					tmp = 0;
				} else {
					tmp = tmp->next;
				}
			}
		} else {
			equals = false;
		}
		return equals;
	}

	/**
		Operatore "!=" di confronto (disuguaglianza) costante.
		Confronta due multiset in base agli elementi da loro contenuti.
		@param other Multiset con cui confrontare this.
		@return bool True se i due multiset sono diversi, false altrimenti.
	*/
	bool operator!= (const multiset &other) const {
		return !(*this == other);
	}

	/**
		Forward Iterator di sola lettura (costante).
		Impedisce (come da specifica) la scrittura degli elementi.
	*/
	class const_iterator;

	class const_iterator {
	private:
		friend class multiset<T>;	///< Imposta la friend con la classe templata multiset per permettere a const_iterator di accedere ai membri privati della suddetta.
		const node *curr;			///< Puntatore al nodo corrente.
		size_type n;				///< Se nel multiset sono presenti pi� elementi uguali "A" (ovvero S = {A,A,A,X...}), allora n tiene in memoria che curr punta all' n-esimo elemento "A". Inizialmente n = 0.

		/**
			Costruttore privato.
			@param ptr Puntatore al nodo a cui punter� curr.
		*/
		const_iterator(const node *ptr) {
			curr = ptr;
			n = 0;
		}

	public:

		/**
			Costruttore di default.
		*/
		const_iterator() : curr(0), n(0) { }

		/**
			Copy Constructor.
			@param other Const_iterator da copiare.
		*/
		const_iterator(const const_iterator &other) : curr(other.curr), n(other.n) { }

		/**
			Operatore Assegnamento.
			@param other Const_iteratore da cui copiare i dati.
			@return this Puntatore all'istanza attuale di const_iterator.
		*/
		const_iterator& operator= (const const_iterator &other) {
			curr = other.curr;
			n = other.n;
			return *this;
		}

		/**
			Distruttore.
		*/
		~const_iterator() { }

		/**
			Dereferenziamento per reference.
			@return T Elemento di tipo T a cui punta l'iteratore.
		*/
		const T &operator*() const {
			return curr->object;
		}

		/**
			Dereferenziamento per puntatore.
			@return T Puntatore all'elemento di tipo T a cui punta l'iteratore.
		*/
		const T * operator->() const {
			return &(curr->object);
		}

		/**
			Operazione "++(int)" di Post-incremento.
			@return tmp Iteratore all'elemento successivo.
		*/
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			if (n == curr->amount - 1) {
				curr = curr->next;
				n = 0;
			} else {
				n++;
			}
			return tmp;
		}

		/**
			Operazione "++" di Pre-incremento.
			@return tmp Iteratore all'elemento successivo.
		*/
		const_iterator& operator++() {
			if (n == curr->amount - 1) {
				curr = curr->next;
				n = 0;
			} else {
				n++;
			}
			return *this;
		}

		/**
			Operatore "==" di confronto (uguaglianza).
			@param other Iteratore da confrontare.
			@return bool True se i due iteratori puntano alla stessa posizione, false altrimenti.
		*/
		bool operator==(const const_iterator &other) const {
			return curr == other.curr && n == other.n;
		}

		/**
			Operatore "==" di confronto (disuguaglianza).
			@param other Iteratore da confrontare.
			@return bool True se i due iteratori puntano ad una differente posizione, false altrimenti.
		*/
		bool operator!=(const const_iterator &other) const {
			return curr != other.curr || n != other.n;
		}
	};

	/**
		Iteratore di inizio sequenza.
		@return const_iterator Iteratore di inizio sequenza.
	*/
	const_iterator begin() const {
		return const_iterator(head);
	}

	/**
		Iteratore di fine sequenza.
		@return const_iterator Iteratore di fine sequenza.
	*/
	const_iterator end() const {
		return const_iterator(0);
	}
};

/**
	Operatore "<<".
	Operatore di stream per la stampa in output di un multiset<T> su un std::ostream.
	@param os Stream di output su cui scrivere.
	@param ms Multiset da stampare su os.
	@return os Stream di output.
*/
template <typename T>
std::ostream & operator<<(std::ostream &os, const multiset<T> &ms) {
	multiset<T>::const_iterator it = ms.begin();
	const T *curr = 0;
	unsigned int x = 1;
	os << "{";
	for (; it != ms.end(); ++it) {
		if (curr != &(*it)) {
			os << "<" << *it << ", " << ms.object_count(*it) << ">"; //(!) Eccezione: Operatore "<<" per un certo oggetto non � specificata.
			if (x < ms.get_nodes_amount()) {
				os << ", ";
			}
			++x;
		}
		curr = &(*it);
	}
	os << "}";
	return os;
}
#endif
