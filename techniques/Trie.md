# Trie (トライ木)

## テンプレート
```C++
struct Node {
    int num;
    char c;
    vector<int> edge;
    // Data data
    Node(){}
    Node(int n, char s) : num(n), c(s) {
        edge.assign(26, -1);
    }

    bool contains(char s){
        if(edge[s-'a'] == -1) return false;
        else return true;
    }

    bool push(char s, int n){
        if(contains(s)) return false;
        edge[s-'a'] = n;
        return true;
    }

    int next(char s){
        return edge[s-'a'];
    }
};

int main(){
    vector<Node> nodes;
    int node_num = 0;

    Node node(node_num, '.');
    nodes.emplace_back(node);
    node_num++;

    auto add_string = [&](auto f, string &str, int d, int n) -> void {
        if(d == str.size()){
            return;
        }
        if(!nodes[n].contains(str[d])){
            Node node(node_num, str[d]);
            nodes.emplace_back(node);
            ndoes[n].push(str[d], node_num);
            node_num++;
        }
        f(f, str, d+1, nodes[n].next(str[d]));
    };
}
```