template <typename T>
class CBTree
{
    //M�� �����Ϊ M���棩�������ں���M-1����������� M ������
    //���ڵ������������ӽڵ�
    //ÿ���ڵ���M - 1��key����������������
     // λ��M - 1��M key���ӽڵ��ֵλ��M - 1 ��M key��Ӧ��Value֮��
    //�����ڵ�������M / 2���ӽڵ�
    //��С����M=2

    static const int M = 2;
    static const int KEY_MAX = 2 * M - 1;
    static const int KEY_MIN = M - 1;
    static const int CHILD_MAX = 2 * M;
    static const int CHILD_MIN = M;

    typedef struct Node {
        int    n;               //�ؼ���ĸ���
        T      k[KEY_MAX];         //�ؼ���
        Node* c[CHILD_MAX];      //�ӽڵ�ָ��
        bool    leaf;            //�Ƿ���Ҷ�ӽڵ�
    } NODE, * PNODE;

public:
    CBTree(void)
    {
        m_pRoot = NULL;
    }
    ~CBTree(void)
    {
        DestoryTree(m_pRoot);
        m_pRoot = NULL;
    }

public:
    bool Insert(T key)
    {
        //�Ѿ������ˣ�ֱ�ӷ��سɹ�
        if (Search(m_pRoot, key))
            return true;

        if (NULL == m_pRoot)
            m_pRoot = CreateEmptyTree();

        if (m_pRoot->n == KEY_MAX)
        {
            PNODE pNew = NewNode();
            pNew->n = 0;
            pNew->leaf = false;
            pNew->c[0] = m_pRoot;
            SplitChild(pNew, 0);
            InsertNoneFull(pNew, key);
            m_pRoot = pNew;
        }
        else
        {
            InsertNoneFull(m_pRoot, key);
        }

        return true;
    }

    bool Delete(T key)
    {
        if (NULL == m_pRoot)
            return true;

        //�����ڣ�����ɾ��
        if (!Search(m_pRoot, key))
            return true;

        if (m_pRoot->n == 1)         //�����������
        {
            if (m_pRoot->leaf)
            {
                DestoryTree(m_pRoot);
                m_pRoot = NULL;
                return true;
            }
            else
            {
                PNODE pC1 = m_pRoot->c[0];
                PNODE pC2 = m_pRoot->c[1];
                if (pC1->n == KEY_MIN && pC2->n == KEY_MIN)
                {
                    MergeChild(m_pRoot, 0);
                    DeleteNode(m_pRoot);
                    m_pRoot = pC1;
                }
            }
        }
        DeleteNoneEmpty(m_pRoot, key);

        return true;
    }

    bool Find(T key)
    {
        return Search(m_pRoot, key);
    }

protected:
    bool Search(PNODE pNode, T key)
    {
        if (pNode == NULL)
            return false;

        int i = 0;
        while (i<pNode->n && key>pNode->k[i])
        {
            i++;
        }

        if (i < pNode->n && (key == pNode->k[i]))
        {
            return true;
        }
        else if (pNode->leaf)
        {
            return false;
        }
        else
        {
            return Search(pNode->c[i], key);
        }
    }

    //��pParent�ĵ�nChildIndex�����ӷ���
    bool SplitChild(PNODE pParent, int nChildIndex)
    {
        PNODE pChild = pParent->c[nChildIndex];
        //Ϊ�·��ѳ��Ľڵ����ռ�
        PNODE pChild2 = NewNode();
        if (NULL == pChild2)
            return false;
        //�뱻���ѵ�ͬ��
        pChild2->leaf = pChild->leaf;
        //���÷��ѽڵ�key��
        pChild2->n = KEY_MIN;

        //��������
        for (int i = 0; i < KEY_MIN; i++)
        {
            pChild2->k[i] = pChild->k[i + M];
        }
        //�������Ҷ�ڵ㣬����ָ��
        if (!pChild->leaf)
        {
            for (int i = 0; i < CHILD_MIN; i++)
            {
                pChild2->c[i] = pChild->c[i + M];
            }
        }
        pChild->n = KEY_MIN;

        //���м�����Ϊ�������뵽˫�׽ڵ���
        //��������Ĺؼ��ֺ�ָ�붼�����ƶ�һ��λ��
        for (int i = pParent->n; i > nChildIndex; i--)
        {
            pParent->k[i] = pParent->k[i - 1];
            pParent->c[i + 1] = pParent->c[i];
        }
        pParent->k[nChildIndex] = pChild->k[M - 1];
        pParent->n++;
        pParent->c[nChildIndex + 1] = pChild2;

        return true;
    }

    void InsertNoneFull(PNODE pNode, T key)
    {
        int i = pNode->n;
        PNODE pChild = NULL;
        if (pNode->leaf)
        {
            while (i > 0 && (key < pNode->k[i - 1]))
            {
                pNode->k[i] = pNode->k[i - 1];
                i--;
            }
            pNode->k[i] = key;
            pNode->n++;
        }
        else
        {
            while (i > 0 && key < pNode->k[i - 1])
            {
                i--;
            }
            pChild = pNode->c[i];
            //����ӽڵ�key����
            if (pChild->n == KEY_MAX)
            {
                //�����ӽڵ�
                SplitChild(pNode, i);
                //������ڷ��Ѻ���������key��������Ѻ���ұߣ��������
                if (key > pNode->k[i])
                {
                    i++;
                    pChild = pNode->c[i];//���Ѻ���Ҳ���
                }
            }
            InsertNoneFull(pChild, key);
        }
    }

    void MergeChild(PNODE pParent, int index)
    {
        PNODE pChild1 = pParent->c[index];
        PNODE pChild2 = pParent->c[index + 1];

        //��c2���ݺϲ���c1
        pChild1->n = KEY_MAX;
        pChild1->k[M - 1] = pParent->k[index];
        for (int i = M; i < KEY_MAX; i++)
        {
            pChild1->k[i] = pChild2->k[i - M];
        }
        if (!pChild1->leaf)
        {
            for (int i = M; i < CHILD_MAX; i++)
            {
                pChild1->c[i] = pChild2->c[i - M];
            }
        }

        //���ڵ�ɾ��index��key��index�����ǰ�ƶ�һλ
        pParent->n--;
        for (int i = index; i < (pParent->n); i++)
        {
            pParent->k[i] = pParent->k[i + 1];
            pParent->c[i + 1] = pParent->c[i + 2];
        }

        //ɾ��child2
        DeleteNode(pChild2);
    }

    void DeleteNoneEmpty(PNODE pNode, T key)
    {
        int i = pNode->n;
        if (pNode->leaf)
        {
            while (i > 0 && (key < pNode->k[i - 1])) { i--; }
            if (key == pNode->k[i - 1])                        //1
            {
                for (int j = i; j < pNode->n; j++)
                {
                    pNode->k[j - 1] = pNode->k[j];
                }
                pNode->n--;
            }
        }
        else
        {
            while (i > 0 && (key < pNode->k[i - 1])) { i--; }
            if (i > 0 && key == pNode->k[i - 1])                        //2
            {
                PNODE pChild1 = pNode->c[i - 1];
                PNODE pChild2 = pNode->c[i];
                //��Child�࣬����Child��һ��
                if (pChild1->n > KEY_MIN)                     //2a
                {
                    T preKey = GetPredecessor(pChild1);
                    DeleteNoneEmpty(pChild1, preKey);
                    pNode->k[i - 1] = preKey;
                }
                //��Child�࣬����Child��һ��
                else if (pChild2->n > KEY_MIN)                  //2b
                {
                    T sucKey = GetSuccessor(pChild2);
                    DeleteNoneEmpty(pChild2, sucKey);
                    pNode->k[i - 1] = sucKey;
                }
                //���Ҷ���M-1���ϲ�
                else                                    //2c
                {
                    MergeChild(pNode, i - 1);
                    DeleteNoneEmpty(pChild1, key);
                }
            }
            else                                       //3
            {
                //����key������
                PNODE pSub = pNode->c[i];
                if (pSub->n == KEY_MIN)
                {
                    PNODE pLeft = (i > 0 ? pNode->c[i - 1] : NULL);
                    PNODE pRight = (i < pNode->n ? pNode->c[i + 1] : NULL);
                    if (pLeft && pLeft->n > KEY_MIN)            //3a1
                    {
                        GetFromLeftBrother(pNode, i - 1, pSub, pLeft);
                    }
                    else if (pRight && pRight->n > KEY_MIN)         //3a2
                    {
                        GetFromRightBrother(pNode, i, pSub, pRight);
                    }
                    else if (pLeft && pLeft->n == KEY_MIN)         //3b1
                    {
                        MergeChild(pNode, i - 1);
                        pSub = pLeft;
                    }
                    else if (pRight && pRight->n == KEY_MIN)      //3b2
                    {
                        MergeChild(pNode, i);
                    }
                }
                DeleteNoneEmpty(pSub, key);
            }
        }
    }
    T GetPredecessor(PNODE pNode)
    {
        while (!pNode->leaf)
        {
            pNode = pNode->c[pNode->n];
        }
        return pNode->k[pNode->n - 1];
    }
    T GetSuccessor(PNODE pNode)
    {
        while (!pNode->leaf)
        {
            pNode = pNode->c[0];
        }
        return pNode->k[0];
    }
    //��left���key��pParent(index)����pParent(index)��pNode
    void GetFromLeftBrother(PNODE pParent, int index, PNODE pNode, PNODE pLeft)
    {
        for (int i = pNode->n; i > 0; i--)
        {
            pNode->k[i] = pNode->k[i - 1];
        }
        pNode->k[0] = pParent->k[index];
        if (!pNode->leaf)
        {
            for (int i = pNode->n; i >= 0; i--)
            {
                pNode->c[i + 1] = pNode->c[i];
            }
            pNode->c[0] = pLeft->c[pLeft->n];
        }
        pNode->n++;

        //parent
        pParent->k[index] = pLeft->k[pLeft->n - 1];

        //left
        pLeft->n--;


    }
    //��right��Сkey��pParent(index)����pParent(index)��pNode
    void GetFromRightBrother(PNODE pParent, int index, PNODE pNode, PNODE pRight)
    {
        pNode->k[pNode->n] = pParent->k[index];
        if (!pNode->leaf)
        {
            pNode->c[pNode->n + 1] = pRight->c[0];
        }
        pNode->n++;

        //parent
        pParent->k[index] = pRight->k[0];

        //right
        for (int i = 1; i < pRight->n; i++)
        {
            pRight->k[i - 1] = pRight->k[i];
        }
        if (!pRight->leaf)
        {
            for (int i = 0; i < pRight->n; i++)
            {
                pRight->c[i] = pRight->c[i + 1];
            }
        }
        pRight->n--;
    }

    //////////////////////////////////////////////////////////////////////////

    PNODE NewNode()
    {
        PNODE pNode = new NODE;
        if (NULL == pNode)
            return pNode;

        //init key count
        pNode->n = 0;

        //init children
        for (int i = 0; i < CHILD_MAX; i++)
            pNode->c[i] = NULL;

        return pNode;
    }
    void DeleteNode(PNODE& pNode)
    {
        if (pNode)
        {
            delete pNode;
            pNode = NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    PNODE CreateEmptyTree() {
        PNODE pRet = NewNode();
        pRet->n = 0;
        pRet->leaf = true;
        return pRet;
    }
    void DestoryTree(PNODE pNode)
    {
        if (pNode)
        {
            for (int i = 0; i <= pNode->n; i++)
            {
                if (pNode->leaf)
                    break;
                if (pNode->c[i])
                    DestoryTree(pNode->c[i]);
            }
            DeleteNode(pNode);
        }
    }

public:
    void Display()
    {
        Display(m_pRoot, 2);
        printf("\n");
    }
private:
    void Display(PNODE t, int level)
    {
        int i = 0;
        if (t)
        {
            while (i < level - 2)
            {
                printf(" ");
                i++;
            }
            printf("[ ");
            for (int i = 0; i < t->n; i++)
            {
                printf("%d ", t->k[i]);
            }
            printf("]\n");

            for (int i = 0; i <= t->n; i++)
            {
                Display(t->c[i], level + 3);
            }
        }
    }

private:
    PNODE m_pRoot;

};