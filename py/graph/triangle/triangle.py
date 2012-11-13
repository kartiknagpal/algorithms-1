import sys

class TriangleGraph(object):
    """
    Problem: Determine the maximum sum of numbers in a path from the
    top of the triangle to the bottom. You have to start at the top
    and move to adjacent numbers on the line below.

    Example triangle (samples/triangle2.txt):

               (5)
               / \
              /   \
            (9)    6
            / \   / \
           /   \ /   \
          4    (6)    8
         / \   / \   / \
        /   \ /   \ /   \
       0    (7)    1     5

    Answer to the triangle above is 5+9+6+7 = 27
    """

    def __init__(self, triangle_lines=[]):
        """
        triangle_lines should be a list of space-delimited strings of numbers:
            Example: ["5", "9 6", "4 6 8", "0 7 1 5"] represents the following:
               5
               9 6
               4 6 8
               0 7 1 5
        """
        self.lines = triangle_lines

        self.graph = []
        self.numNodes = 0

        self.values = []
        self.distances = []
        self.predecessors = []

        self.build_adjacency_lists()

    def build_adjacency_lists(self):
        """
        Generates the adjacency lists from the data.
        """
        self.numNodes = 0
        for line in self.lines[:-1]:
            line = line.split()
            c = len(line)
            for v in line:
                self.values.append(int(v))
                self.graph.append((self.numNodes+c, self.numNodes+c+1))
                self.numNodes += 1

        line = self.lines[-1].split()
        self.values.extend([int(v) for v in line])
        self.graph.extend([() for v in line])
        self.numNodes += len(line)

    def path(self, s, v):
        """
        Returns the total cost of traveling from s to v.
        """
        if s == v:
            return self.values[s]
        elif self.predecessors[v] == -1:
            return 0
        else:
            return self.path(s, self.predecessors[v]) + self.values[v]

    def dijkstra(self, s):
        """
        Dijkstra's algorithm finds the single-source shortest-path in a
        weighted directed graph. I want to find the single-source
        longest-path, so rather than initialize distances to infinity,
        they're initialized to -1.

        I honestly don't think this can be called dijkstra anymore since it
        doesn't require the usual pieces from Dijkstra's algorithm. The graph
        is basically a pipeline/topology getting processed in order from top to
        bottom. You don't need the usual graph traversal stuff like a queue and
        vertex state maintenance.
        """
        for i in xrange(self.numNodes):
            self.distances.append(-1)
            self.predecessors.append(-1)
        self.distances[s] = 0

        for u in xrange(self.numNodes):
            for v in self.graph[u]:
                w = self.values[u] + self.values[v]
                if self.distances[v] < self.distances[u] + w:
                    self.distances[v] = self.distances[u] + w
                    self.predecessors[v] = u

    def longest_path(self):
        total = 0
        for i in xrange(self.numNodes-len(self.lines), self.numNodes):
            # iterate over the last line's numbers from start to end to
            # get valid end points.
            t = self.path(0, i)
            if t > total:
                total = t
        return total

def main():
    if len(sys.argv) < 2:
        print "Usage: %s <triangle_file>" % sys.argv[0]
        return

    try:
        lines = open(sys.argv[1]).readlines()
    except IOError:
        print "Can't open file '%s'" % sys.argv[1]
        return

    lines = [line.strip() for line in lines]

    G = TriangleGraph(lines)
    G.dijkstra(0)
    print G.longest_path()

if __name__ == "__main__":
    main()
