# Autocomplete

Autocomplete in C

When you type into a search bar, maps app, or contact picker, suggestions appear after a few letters—autocomplete. Behind the scenes, systems look up all entries that start with your prefix and rank them so the most relevant (e.g., most frequent, most popular) rise to the top.
This project is a tiny C implementation of that idea: it loads a file of (weight, term) pairs (e.g., word frequency or city population), finds all terms beginning with a given prefix, and returns them sorted by descending weight. It’s small enough to read in one sitting and drop into other C projects.

How it works

Load a text file of (weight, term) pairs into memory.

Sort the array lexicographically by term once at startup.

Use binary search to find the lowest and highest indices that match the prefix.

Copy that slice and sort it by descending weight to produce final suggestions.
