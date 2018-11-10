package Algorithm;

import java.util.LinkedList;
import java.util.NoSuchElementException;


class Queue1<T> {
	
	private Node<T> first;
	private Node<T> last;
	
	class Node<T> {
		private T data;
		private Node<T> next;

		public Node(T data) {
			this.data = data;
		}
	}

	public void enqueue(T item) {
		Node<T> t = new Node<T>(item);

		if (last != null) {
			last.next = t;
		}
		
		last = t;

		if (first == null) {
			first = last;
		}
	}

	public T dequeue() {
		
		if (first == null) {
			throw new NoSuchElementException();
		}

		T data = first.data;
		first = first.next;

		if (first == null) {
			last = null;
		}
		return data;
	}

	public T peek() {
		if (first == null) {
			throw new NoSuchElementException();
		}
		return first.data;
	}

	public boolean isEmpty() {
		return first == null;
	}
}

class Graph{
	class Node{
		int data;
		LinkedList<Node> adjacent;
		boolean marked;
		Node (int data){
			this.data = data;
			this.marked = false;
			adjacent = new LinkedList<Node>();
		}
	}
	
	Node[] nodes;
	
	Graph(int size) {
		nodes = new Node[size];
		for (int i =0 ; i < size; i++) {
			nodes[i] = new Node(i);
		}
	}
	
	void addEdge(int i1, int i2) {
		Node n1 = nodes[i1];
		Node n2 = nodes[i2];
		
		if(!n1.adjacent.contains(n2)) {
			n1.adjacent.add(n2);
		}
		if(!n2.adjacent.contains(n1)) {
			n2.adjacent.add(n1);
		}
	}
	
	/**
	 * DFS
	 */
	void dfs() {
		dfs(0);
	}
	
	void dfs(int index) {
		Node root = nodes[index];
		Stack<Node> stack = new Stack<Node>();
		stack.push(root);
		root.marked = true;
		while(!stack.isEmpty()) {
			Node r = stack.pop();
			for(Node n : r.adjacent) {
				if(n.marked == false) {
					n.marked = true;
					stack.push(n);
				}
			}
			visit(r);
		}
	}
	
	/**
	 * 재귀 호출 DFS
	 */
	void dfsR(Node r) {
		if ( r == null) return;
		r.marked = true;
		visit(r);
		for(Node n : r.adjacent) {
			if(n.marked == false) {
				dfsR(n);
			}
		}
	}
	void dfsR(int index) {
		Node r = nodes[index];
		dfsR(r);
	}
	
	void dfsR() {
		dfsR(0);
	}
	
	void bfs() {
		bfs(0);
	}
	
	void bfs(int index) {
		Node root = nodes[index];
		Queue1<Node> queue = new Queue1<Node>();
		queue.enqueue(root);
		
		root.marked = true;
		
		while(!queue.isEmpty()) {
			Node r = queue.dequeue();
			for(Node n : r.adjacent) {
				if(n.marked == false) {
					n.marked = true;
					queue.enqueue(n);
				}
			}
			visit(r);
		}
	}
	void visit(Node n) {
		System.out.print(n.data + " ");
	}
}
/*
 	 0
 	/
 1 ㅡ 3       7
 |   / |  \   /
 | /   |    5
 2 ㅡ 4     \
 				6 ㅡ 8
 */              
public class DfsBfsTest {
	public static void main(String[] args) {
		Graph g = new Graph(9);
		g.addEdge(0, 1);
		g.addEdge(1, 2);
		g.addEdge(1, 3);
		g.addEdge(2, 4);
		g.addEdge(2, 3);
		g.addEdge(3, 4);
		g.addEdge(3, 5);
		g.addEdge(5, 6);
		g.addEdge(5, 7);
		g.addEdge(6, 8);
		//g.dfs();
		g.bfs();
	}
}
