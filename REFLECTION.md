1. For every function you wrote (TODO 1 to 19), explain in your own words what it does and
   give its time complexity in terms of n, the number of scenes. One short paragraph each.

*NOTE* FOR QUESTION 1: n represents the number of scenes

TODO 1: I implemented the constructor of Scene() which dynamically allocates and initializes choices for a 
non-ending scene. O(1) (though num choices is capped at 3, which is still 
technically O(1)).

TODO 2: I implemented the destructor of Scene which destroys each child scene, starting with the pointers, then the 
array of choices, then the data object owned by the scene. O(n).

TODO 3: I implemented isEnding in Scene.h which checks the number of children, if there are no children, return true;
it is a leaf (ending). O(1).

TODO 4: I implemented getChoice in Scene.h which returns the child scene object at index value passed as a parameter. 
O(1).

TODO 5: I implemented setChoice in Scene.h which deletes and replaces the old child scene and an index value
with a scene passed as a parameter. O(n).


TODO 6: I implemented countScenes() in StoryTree.h which recursively sums all the scenes in the entire story. O(n)

TODO 7: I implemented countEndings() in StoryTree.h which recursively sums all the leaf (ending) scenes in the tree. 
O(n).

TODO 8: I implemented height in StoryTree.h which recursively stores the height for each subtree of the story, then 
finds the maximum height for each possible subtree (at most 3 subtrees). O(n)

TODO 9:I implemented clear() in StoryTree.h which deletes the entire tree by deleting the root, then setting it to 
null ptr to avoid memory leaks. O(n).

TODO 10: I implemented load() which opens the file and creates the entire tree through the recursive function 
readScene() even though load only technically acts as a wrapper for readScene(). O(n).

TODO 11: I implemented readScene() which reads each part of the scene through the readField() helper method, then 
recursively reads and attaches next scene if the next scene exists. O(n).

TODO 12: I implemented askChoice() which asks the user for an integer input, the function prints an error message if
anything goes wrong when reading user input (input mismatch, error when parsing from string to int, etc.). O(1) per 
input attempt.

TODO 13: I implemented play() which prints the scene data(including hp, loot, item pick up, item inventory, etc.) 
and gets the user choice for the next scene via askChoice(). Then the function walks to the next scene. Although the 
path traversal is O(n), repeatedly searching the growing inventory can take O(n²) in the worst case.

TODO 14: I implemented printStats() which calls countScenes(), countEndings(), and height() to print to the console. 
O(n).

TODO 15: I implemented printSpoilers() which essentially acts as a wrapper function for printEndings(). O(n).

TODO 16: I implemented printEndings() which recursively adds each choice number to a path string which grows on each
recursion. O(n).

TODO 17: I implemented greedyRun() which prints the path of choices where the loot in the next choice is the greatest
possible loot for all choices in the current scene, regardless of risk to hp. O(n).

TODO 18: I implemented bestLoot() which gets the path of the highest loot where the player remains alive and reaches an
ending. O(n)

TODO 19: I implemented optimalRun() which acts as a wrapper for bestLoot and prints the path saved and integer returned
by bestLoot(). O(n).

2. Why does each scene allocate exactly numChoices slots instead of declaring Scene<T>*
   choices[3]? What does an ending hold in choices_, and why?

The scene dynamically allocates choices_ to size numChoices_ because the number of given choices for a scene is not 
known until runtime. Hardcoding numChoices to exactly 3 every time would force each scene to allocate three choices 
instead of the actual number of choices held by a scene. An ending has nullptr choices_ because an ending doesn't have
any valid choices and therefore ends the story.

3. Walk through your Scene destructor. What gets deleted, and in what order? What would
   go wrong if choices_ were deleted first?

First, each child choice is deleted, then the array is deleted, then the data owned by the scene is deleted. If the
choices_ array was deleted first, it would cause the children scenes to become unreachable through choices_[i]. 
Accessing choices_[i] after deleting the array would result in undefined behavior. Reversing the order of delete[] 
choices_ and delete choices_[i] leaks memory.

4. Show the path greedy took and the path optimal took on your story, with the loot for each.
   Explain why greedy lost. If greedy did not lose, change your story until it does, and explain
   what you changed.

Greedy advisor
Path: 1-2-1
Loot: 1 + 4 + 1 = 6
Result: died

Optimal advisor
Path: 1-1-2-2-1
Loot: 1 + 3 + 2 + 1 + 100 = 107
Result: reached an ending

I made the greedy path lose because the choice in a scene with the highest immediate loot brings you closer
to a bad ending where health reaches 0 or lower (except for the first scene and optimal ending). Alternatively,
the optimal path had slightly lower immediate rewards to reach a much greater loot in the end. 

5. bestLoot tries every path, yet it runs in O(n) time. Explain why it is not exponential, using
   what you know about paths and leaves in a tree.

The time complexity is O(n) when traversing the tree. This is because the tree has n scenes, and bestLoot() visits 
every scene connected to the root once, meaning it is O(n) and not exponential. Though it considers the traversal of 
each path from root to leaf, each path is tied to the root and ends at a leaf, and there are at most n leaves. The 
paths can share scenes, but bestLoot() visits each shared scene only once instead of traversing each entire path 
separately.

6. Which List methods does your game use, and why? What does one search cost as the bag
   grows?

My List methods (functions) used in Adventure.cpp include, search(), size(), addFront(), and print(). I used search()
to determine if an item already existed in the inventory list, size() to print the number of total items, addFront() to
add an item to the list, and print to print the inventory information. The time complexity for search is O(n) because it
performs a linear search. If the bag has n items, a linear search may examine all n items, so one search costs O(n). In 
the worst case, this search occurs throughout an O(n)-scene playthrough, making play() O(n²).