import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;

/* A directed graph with nodes and edges.  We are not going to allow
 * duplicate edges.
 */
public class Graph {
    /*
     * A node. Contains a name, a payload, and edges.
     */
    private class Node {
        public String name;
        public Object payload;

        // Map of edges, from name to weight.
        private HashMap<String, Integer> edges;

        // Distance for Dijkstra's algorithm
        // This is initialized to Integer.MAX_VALUE, and will be updated
        // during the algorithm.
        public int distance;

        // Origin for Dijkstra's algorithm
        // This will be set to the name of the node from which we reached this node
        // with the shortest path.
        public String origin;

        /*
         * Connect this node to node to. Returns false if to is not in the
         * graph, or if they were already connected; returns true if they
         * are successfully connected.
         */
        public boolean connect(String to, Integer weight) {
            if (!nodes.containsKey(to)) {
                return false;
            }

            Integer result = edges.put(to, weight);
            return (result == null);
        }

        /*
         * Disconnect from node to. Returns true if it disconnects them,
         * false if they weren't connected.
         */
        public boolean disconnect(String to) {
            return (edges.remove(to) != null);
        }

        /*
         * Check whether we have an edge to node to.
         */
        public boolean connected(String to) {
            return edges.containsKey(to);
        }

        /*
         * Return the weight of the edge to node to, or null if they're
         * not connected.
         */
        public Integer edgeWeight(String to) {
            return edges.get(to);
        }

        /*
         * Change our data value.
         */
        public void changeNodeValue(Object value) {
            payload = value;
        }

        /*
         * Print our name, value and connections.
         */
        public void printNode() {
            System.out.printf("Node %s with value %s.\n", name, payload.toString());

            for (String to : edges.keySet()) {
                Integer weight = edges.get(to);

                System.out.printf("  Connected to %s at weight %d.\n", to, weight);
            }
        }

        // ----------------------------------------------------------- //
        // -------------------- Start of my code --------------------- //

        // Getter for the distance, used in Dijkstra's algorithm
        // This will return the distance from the starting node to this node.
        public int getDistance() {
            return distance;
        }

        // Setter for the distance, used in Dijkstra's algorithm
        // This will set the distance from the starting node to this node.
        public void setDistance(int distance) {
            this.distance = distance;
        }

        // Getter for the origin, used in Dijkstra's algorithm
        // This will return the name of the node from which we reached this node
        // with the shortest path.
        public String getOrigin() {
            return origin;
        }

        // Setter for the origin, used in Dijkstra's algorithm
        // This will set the name of the node from which we reached this node
        // with the shortest path.
        public void setOrigin(String origin) {
            this.origin = origin;
        }

        /*
         * Constructor. Needs the index name and the payload object.
         * If this is a simple graph, the payload can be null.
         * // Added Distance;
         */
        public Node(String newName, Object newPayload) {
            edges = new HashMap<String, Integer>();

            name = newName;
            payload = newPayload;
            distance = Integer.MAX_VALUE; // Initialize distance for Dijkstra's algorithm

        }
    }

    /*
     * The collection for the nodes. We index the nodes by name.
     */
    HashMap<String, Node> nodes = new HashMap<String, Node>();

    /*
     * Check whether we have an edge between two nodes.
     */
    public boolean connected(String from, String to) {
        if (!nodes.containsKey(from) || !nodes.containsKey(to)) {
            return false;
        }

        return nodes.get(from).connected(to);
    }

    /*
     * Return the weight of the edge between two nodes, or null if they're
     * not connected.
     */
    public Integer edgeWeight(String from, String to) {
        if (!nodes.containsKey(from) || !nodes.containsKey(to)) {
            return null;
        }

        return nodes.get(from).edgeWeight(to);
    }

    /*
     * Connect nodes from and to. Returns false if either node is not in the graph,
     * or if they were already connected; returns true if they are successfully
     * connected.
     */
    public boolean connect(String from, String to, Integer weight) {
        if (!nodes.containsKey(from) || !nodes.containsKey(to)) {
            return false;
        }

        return nodes.get(from).connect(to, weight);
    }

    /*
     * Disconnect nodes from and to. Returns true if it disconnects them,
     * false if they weren't connected.
     */
    public boolean disconnect(String from, String to) {
        if (!nodes.containsKey(from) || !nodes.containsKey(to)) {
            return false;
        }

        return nodes.get(from).disconnect(to);
    }

    /*
     * Create a new node with associated data value. Returns true if the node is
     * successfully created, false if a node with that name already existed.
     */
    public boolean addNode(String name, Object value) {
        if (nodes.containsKey(name)) {
            return false;
        }

        Node n = new Node(name, value);
        nodes.put(name, n);
        return true;
    }

    // Returns true if and only if we have the named node.
    public boolean hasNode(String name) {
        return nodes.containsKey(name);
    }

    /*
     * Returns the data value of the named node, or null if there is no node
     * with that name.
     */
    public Object nodeValue(String name) {
        if (!nodes.containsKey(name)) {
            return null;
        }

        return nodes.get(name).payload;
    }

    /*
     * Change the data value of a node in a graph. Returns true if the value is
     * successfully changed, false if the node doesn't exist.
     */
    public boolean changeNodeValue(String name, Object value) {
        if (!nodes.containsKey(name)) {
            return false;
        }

        nodes.get(name).changeNodeValue(value);
        return true;
    }

    /*
     * Print the name, value and connections of a given node.
     */
    public void printNode(String name) {
        if (!nodes.containsKey(name)) {
            System.out.printf("No node named %s.\n", name);
            return;
        }

        nodes.get(name).printNode();
    }
    // ----------------------------------------------------------- //
    // -------------------- Start of my code --------------------- //

    // A class to represent an edge in the graph.
    public static class Edge {
        String from;
        String to;
        int weight;

        Edge(String from, String to, int weight) {
            this.from = from;
            this.to = to;
            this.weight = weight;
        }
    }

    /*
     * Dijkstra's algorithm.
     */
    public static void dijkstra(Graph g, String start) {

        PriorityQueue<Node> pq = new PriorityQueue<>(
                (a, b) -> Integer.compare(a.distance, b.distance));

        g.nodes.get(start).setDistance(-1);
        g.nodes.get(start).setOrigin("-1");

        pq.add(g.nodes.get(start));

        while (!pq.isEmpty()) {
            //  Get smallest distance node
            Node curr = pq.poll();
            //  Add all edges to the node
            for (Map.Entry<String, Integer> entry : curr.edges.entrySet()) {
                String neighborName = entry.getKey();
                int weight = entry.getValue();

                Node neighbor = g.nodes.get(neighborName);
                if (neighbor == null) continue;

                //  Calculate new distance
                int newDistance = curr.getDistance() + weight;

                //  If the new distance is smaller, update the neighbor's distance and origin
                if (newDistance < neighbor.getDistance()) {
                    neighbor.setDistance(newDistance);
                    neighbor.setOrigin(curr.name);
                    pq.add(neighbor);
                }
            }
            //  Print the current node's distance and origin
            System.out.printf("Node %s: Distance = %d, Origin = %s\n",
                    curr.name, curr.getDistance(), curr.getOrigin());
            
                    
        }
    }

    public static void main(String[] args) throws Exception {

        // Check if a filename is provided as a command-line argument
        if (args.length == 0) {
            System.out.println("Usage: java ReadFileWithScanner <filename>");
            return;
        }
        // Read the filename from the command-line arguments
        String filename = args[0];

        // Validate the filename
        if (filename == null || filename.isEmpty()) {
            System.err.println("Error: Filename cannot be null or empty.");
            return;
        }
        if (!new File(filename).exists()) {
            System.err.println("Error: File does not exist - " + filename);
            return;
        }

        // Create a new graph instance
        Graph g = new Graph();

        // Read the file and populate the graph
        Scanner scanner = new Scanner(new File(filename));
        // Read the first three lines of the file
        int nodeCount = Integer.parseInt(scanner.nextLine());
        String startingNode = "node" + scanner.nextLine();
        int edgeCount = Integer.parseInt(scanner.nextLine());

        // Add nodes to the graph based on the node count
        for (int i = 1; i <= nodeCount; i++) {
            g.addNode("node" + Integer.toString(i), i); // Node name: "1", "2", ...
        }

        // Read each edge and add it to the graph
        for (int i = 0; i < edgeCount; i++) {
            String line = scanner.nextLine();
            String[] parts = line.split(" ");
            String fromNode = parts[0];
            String toNode = parts[1];
            int weight = Integer.parseInt(parts[2]);
            g.connect("node" + fromNode, "node" + toNode, weight);
        }
        scanner.close();

        dijkstra(g, startingNode);

    }
}
