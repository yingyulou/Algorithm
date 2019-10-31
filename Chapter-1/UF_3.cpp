#include <iostream>
#include <vector>

using namespace std;

class UF
{
public:

    UF(int N): __count(N)
    {
        for (int i = 0; i < N; i++)
        {
            __id.push_back(i);
            __sz.push_back(1);
        }
    }

    void _union(int p, int q)
    {
        int pRoot = find(p), qRoot = find(q);

        if (pRoot != qRoot)
        {
            if (__sz[pRoot] < __sz[qRoot])
            {
                __id[pRoot] = qRoot;
                __sz[qRoot] += __sz[pRoot];
            }
            else
            {
                __id[qRoot] = pRoot;
                __sz[pRoot] += __sz[qRoot];
            }

            __count--;
        }
    }

    int find(int p) {

        while (p != __id[p])
        {
            p = __id[p];
        }

        return p;
    }

    bool connected(int p, int q) { return find(p) == find(q); }

    int count() { return __count; }

private:

    vector<int> __id, __sz;
    int __count;
};

int main()
{
    FILE *f = fopen("largeUF.txt", "r");
    int N, p, q;

    fscanf(f, "%d", &N);

    UF uf(N);

    for (int _ = 0; _ < N; _++)
    {
        fscanf(f, "%d %d", &p, &q);

        if (!uf.connected(p, q))
        {
            uf._union(p, q);
        }
    }

    fclose(f);
}