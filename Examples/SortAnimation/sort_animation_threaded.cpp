// sort_animation_threaded.cpp
// Created 2/12/2019 1:27:41 PM

#include <GL/sgl.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <iostream>


const int ARRAY_SIZE = 600;


class AnimationWindow : public sgl::Window {
protected:

	int delay;       // Controls the speed of the sort
	bool paused;     // True, if a sort in progress is paused; otherwise, false
	bool sorting;    // True if a sort is in progress; otherwise, false

	// Allows a sort method to update the display during some part of its 
	// execution.  Places the sort into a busy wait if the sorting is paused.
	// (Spacebar toggles pausing.)
	// Returns an indication of whether the sort should continue (true) or
	// terminate (false).  The user controls the sorting instance variable
	// through the user interface (press the escape key to stop sorting).
	bool snapshot() {
		using namespace std::chrono_literals;
		repaint();
		sgl::pause(delay);
		auto start = std::chrono::high_resolution_clock::now();
		// Note: We do something in this loop so the optimizer will not
		// remove this code
		while (paused)
			std::this_thread::sleep_for(0.5s); 
		return sorting;
	}

	// Scrambles the contents of the vector vec pseudorandomly.
	void random_permute(std::vector<int>& vec) {
		int n = vec.size();
		for (int i = 0; i < n - 1; i++) {
			int pos = sgl::random(i, n - 1);
			std::swap(vec[i], vec[pos]);
		}
	}


	// Sorting functions
	
	// Standard selection sort: swaps minimal element
	// with current position after all remaining elements
	// are considered.
	void selection_sort(std::vector<int>& vec) {
		int n = vec.size();
		for (int i = 0; i < n - 1; i++) {
			int smallest = i;
			for (int j = i + 1; j < n; j++)
				if (vec[j] < vec[smallest])
					smallest = j;
			if (!snapshot()) return;
			if (i != smallest)
				std::swap(vec[i], vec[smallest]);
		}
	}


	// Simpler (but less efficient) selection sort: eagerly 
	// swaps smaller elements with current position as 
	// encountered during the scan of the remaining elements.
	void eager_selection_sort(std::vector<int>& vec) {
		int n = vec.size();
		for (int i = 0; i < n - 1; i++) {
			if (!snapshot()) return;
			for (int j = i + 1; j < n; j++)
				if (vec[j] < vec[i])
					std::swap(vec[i], vec[j]);
		}
	}

	// Standard insertion sort.
	void insertion_sort(std::vector<int>& vec) {
		int n = vec.size();
		for (int i = 1; i < n; i++) {
			int key = vec[i];
			int position = i;
			//  Shift larger values to the right
			while (position > 0 && vec[position - 1] > key) {
				vec[position] = vec[position - 1];
				position--;
			}
			/*if (isSwapVisible()) {
				showTrace(i, key, position, key);
			}*/
			vec[position] = key;

			if (!snapshot()) return;
		}
	}

	// Standard exchange sort.
	void exchange_sort(std::vector<int>& vec) {
		int n = vec.size();
		int k = 0;
		bool sorted = false;
		while (k < n - 1 && !sorted) {
			sorted = true;
			for (int i = 0; i < n - k - 1; i++) {
				if (vec[i] > vec[i + 1]) {
					std::swap(vec[i], vec[i + 1]);
					sorted = false;
				}
			}
			if (!snapshot()) return;

			k++;
		}
	}


	//////////////////////////////////////////////////////////////////////
	//  Heapsort functions
	void siftDown(std::vector<int>& vec, int start, int end) {
		//  End represents the limit of how far down the heap to sift
		int root = start;

		while ((root * 2 + 1) <= end) {  //  Loop while the root has at least one child
			int child = root * 2 + 1;    //  root * 2 + 1 points to the left child
			//  If the child has a sibling, is the child's less than its sibling?
			if (child + 1 <= end && vec[child] < vec[child + 1])
				child = child + 1;       //  Point to the right child instead
			if (vec[root] < vec[child]) {    //  Out of max-heap order
				std::swap(vec[root], vec[child]);
				root = child;    //  Repeat to continue sifting down the child now
			}
			else
				return;
		}
	}

	void heapify(std::vector<int>& vec, int count) {
		//  Start is assigned the index in vec of the last parent node
		int start = (count - 2) / 2; //  Binary heap

		while (start >= 0) {
			//  Sift down the node at index start to the proper place
			//  such that all nodes below the start index are in heap
			//  order
			siftDown(vec, start, count - 1);
			start--;
		}
		//  After sifting down the root all nodes/elements are in heap order
	}

	void heapsort(std::vector<int>& vec) {
		int count = vec.size();

		//  First build the heap
		heapify(vec, count);

		int end = count - 1;
		while (end > 0) {
			//  Swap the root (maximum value) of the heap with the
			//  last element of the heap
			std::swap(vec[0], vec[end]);
			//  Put the heap back in max-heap order
			siftDown(vec, 0, end - 1);
			//  Decrement the size of the heap so that the previous
			//  max value will stay in its proper place
			end--;

			if (!snapshot()) return;
		}
	}

	/////////////////////////////////////////////////////////////////////
	//  Quicksort functions
	void qsort(std::vector<int>& vec, int l, int r) {
		int i = l, j = r, x;
		x = vec[(l + r) / 2];
		do {
			while (vec[i] < x)
				i++;
			while (x < vec[j])
				j--;
			if (i <= j) {
				std::swap(vec[i], vec[j]);
				i++;
				j--;
			}
			/*if (isSwapVisible() && !showSnapshot())
				return;*/
		} while (i <= j);

		//if ( !showSnapshot() ) return;
		if (!snapshot()) return;

		if (l < j)
			qsort(vec, l, j);
		if (i < r)

			qsort(vec, i, r);
	}

	void quicksort(std::vector<int>& vec) {
		qsort(vec, 0, vec.size() - 1);
	}

	// Nested local type for a popup menu of sort choices
	class SortPopup : public sgl::PopupMenu {
		AnimationWindow *parent_window;
	public:
		//SortPopup(AnimationWindow *parent_window) : sgl::PopupMenu(parent_window) {
		SortPopup(AnimationWindow *window) : parent_window(window) {
			add_menu_item("Randomly Permute", [this]() { parent_window->permute(); });
			add_menu_item("Selection Sort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::selection_sort);
				t.detach();
			});
			add_menu_item("Eager Selection Sort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::eager_selection_sort);
				t.detach();
			});
			add_menu_item("Insertion Sort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::insertion_sort);
				t.detach();
			});
			add_menu_item("Exchange Sort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::exchange_sort);
				t.detach();
			});
			add_menu_item("Heapsort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::heapsort);
				t.detach();
			});
			add_menu_item("Quicksort", [this]() {
				std::thread t(&AnimationWindow::do_sort, parent_window, &AnimationWindow::quicksort);
				t.detach();
			});
			add_menu_item("Quit", [this]() { parent_window->quit(); });
		}
	};

	// Nested local type for a popup menu of actions to perform while sorting
	class ActionPopup : public sgl::PopupMenu {
		AnimationWindow *parent_window;
	public:
		ActionPopup(AnimationWindow *window) : parent_window(window) {
			add_menu_item("Pause/Resume Sort", [this]() { parent_window->do_pause_sorting(); });
			add_menu_item("Stop Sort", [this]() { parent_window->do_stop_sorting(); });
			add_menu_item("Speed Up Sort", [this]() { parent_window->do_sort_faster(); });
			add_menu_item("Slow Down Sort", [this]() { parent_window->do_sort_slower(); });
			add_menu_item("Quit", [this]() { parent_window->quit(); });
		}
	};

	std::vector<int> array;          // The sequence to sort
	ActionPopup *action_popup;       // The popup menu object for sort choice
	SortPopup *sort_popup;           // The popup menu object for doing things during a sort

	const std::string TITLE;
	const std::string BUSY_TITLE;

public:
	AnimationWindow(int size) :
		Window("Sort Algorithms", 50, 50, size + 50, size + 50,
			-25.0, size + 25.0, -25.0, size + 25.0),
		delay(10), paused(false), sorting(false),
		action_popup(new ActionPopup(this)),
		sort_popup(new SortPopup(this)),
	    TITLE("Sort Algorithms"), BUSY_TITLE("Sort Algorithms (SORTING)") {
		for (int i = 0; i < size; i++)
			array.push_back(i);
	}

	~AnimationWindow() {
		delete sort_popup;   // Free up popup menus allocated by the constructor
		delete action_popup; 
	}


	void paint() override {
		draw_axes(50.0, 50.0);  
		sgl::set_color(sgl::BLUE);
		sgl::set_point_size(4);
		// Plot the points in the vector
		for (unsigned i = 0; i < array.size(); i++)
			//fill_rectangle(i - 3, array[i] - 3, 6.0, 6.0);
			sgl::draw_point(i, array[i]);
		//  Draw delay indicator (green line under the x-axis.
		sgl::set_color(sgl::GREEN);
		//fill_rectangle(delay, -10.0, 5.0, 3.0);
		sgl::fill_rectangle(10.0, -10.0, delay, 3.0);
		// Draw a red border if paused
		if (paused) {
			sgl::set_color(sgl::RED);
			sgl::set_line_width(10);
			sgl::draw_rectangle(-23.0, -23.0, get_width() - 3.0, get_height() - 3.0);
			sgl::set_line_width(1);
		}
	}

	// How to respond to user's keypresses.
	void key_pressed(int key, double x, double y) override {
		sgl::Window::key_pressed(key, x, y);

		switch (key) {
			case 'S':   // Scramble the contents of the sequence
			case 's':
				random_permute(array);
				break;
			case '<':   // Reduce the delay
			case ',':
				do_sort_faster();
				break;
			case '>':   // Increase the delay
			case '.':
				do_sort_slower();
				break;
			case ' ':   // Toggle sort pause
				do_pause_sorting();
				break;
			case '1':
				//do_selection_sort();
			    {
				    std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::selection_sort);
				    t.detach();
			    }
				break;
			case 27:    // Esc key terminates the sort
				do_stop_sorting();
				break;
			case 'Q':   // Quit the program
			case 'q':
				quit();
				break;
			default:
				break;
		}
		repaint();
	}

	// Ensure a square window: width and height must be identical.
	virtual void resized(int width, int height) {
		if (width != height) {
			(width < height) ? height = width : width = height;
			set_window_size(width, height);
		}
		Window::resized(width, height);
	}




	////////////////////////////////////////////////////////////////

	// Window method that permutes the sequence
	void permute() {
		random_permute(array);
		repaint();
	}

	// Sorts the sequence given algorithm f.  
	// This method should be run in a separate thread so that the user
	// can intervene (pause or cancel a sort) via the user interface 
	// during an active sorting process.
	void do_sort(void (AnimationWindow::*f)(std::vector<int>&)) {
		set_title(BUSY_TITLE);     // Feedback to user that we are sorting
		sorting = true;            // Alter the system's state to
		paused = false;            // indicate we are sorting
		action_popup->activate();  // When sorting we have a different popup menu
		(this->*f)(array);         // Do the sort
		sorting = false;           // Restore the system's state to non-sorting
		paused = false; 
		sort_popup->activate();    // Restore the previous popup menu
		set_title(TITLE);          // Title bar indicates sort is over
	}

	//////////////////////////////////////////////////////////////////////////////////
	//// This section contains the callbacks for the sort menu

	//// Executes the selection sort algorithm on a separate thread.
	//void do_selection_sort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::selection_sort);
	//	t.detach();
	//}


	//// Executes the eager selection sort algorithm on a separate thread.
	//void do_eager_selection_sort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::eager_selection_sort);
	//	t.detach();
	//}

	//// Executes the insertion sort algorithm on a separate thread.
	//void do_insertion_sort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::insertion_sort);
	//	t.detach();
	//}

	//// Executes the exchange sort algorithm on a separate thread.
	//void do_exchange_sort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::exchange_sort);
	//	t.detach();
	//}

	//// Executes the heapsort algorithm on a separate thread.
	//void do_heap_sort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::heapsort);
	//	t.detach();
	//}

	//// Executes the quicksort algorithm on a separate thread.
	//void do_quicksort() {
	//	std::thread t(&AnimationWindow::do_sort, this, &AnimationWindow::quicksort);
	//	sorting = true;
	//	t.detach();
	//}


	////////////////////////////////////////////////////////////////////////////////
	// This section contains the callbacks for the action menu (active during sorting

	// Pauses an active sorting process or 
	// resumes executing a currently paused sort.
	void do_pause_sorting() {
		paused = !paused;
		repaint();
	}


	// Increases the speed of the sort.
	void do_sort_faster() {
		delay = (delay >= 5) ? delay - 5 : delay;
		repaint();
	}

	// Reduces the speed of the sort.
	void do_sort_slower() {
		delay = (delay < 2000) ? delay + 5 : delay;
		repaint();
	}

	// Terminates an active sort process.
	void do_stop_sorting() {
		sorting = false;
		paused = false;
		repaint();
	}

	////////////////////////////////////////////////////////////////////////////////
	// Both the sort menu and action menu can access the quit method

	// Terminates the program's execution.
	void quit() {
		exit(0);
	}
};



int main() {
	sgl::set_random_seed();
	sgl::version();
	std::cout << "Right click on the window for a sorting menu\n";
	std::cout << "Keyboard commands:\n";
	std::cout << "   < or ,    Speed up the sorting speed\n";
	std::cout << "   > or .    Slow down the sorting speed\n";
	std::cout << "   Space     Pause (or unpause) the sorting\n";
	std::cout << "   Esc       Terminate the sort\n";
	std::cout << "   Q or q    Quit the program\n";
	sgl::run<AnimationWindow>(ARRAY_SIZE);
}

