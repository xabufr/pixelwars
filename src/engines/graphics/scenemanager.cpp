#include "scenemanager.h"
#include "engines/graphics/graphicalengine.h"
#include "core/logger.h"

SceneManager::SceneManager()
{
    m_rootSceneNode = new SceneNode(this);
}

SceneManager::~SceneManager()
{
    delete m_rootSceneNode;
}
SceneNode *SceneManager::GetRootNode()
{
    return m_rootSceneNode;
}
void SceneManager::Draw()
{
    std::map<int,std::vector<SceneNode*>>::iterator it;
    sf::RenderWindow *app = GraphicalEngine::GetInstance()->GetRenderWindow();
    for(it=m_nodesLevel.begin();it!=m_nodesLevel.end();it++)
    {
        const std::vector<SceneNode*> &vecSceneNode = it->second;
        unsigned int taille = vecSceneNode.size();
        for(unsigned int i=0;i<taille;++i)
        {
            const std::vector<SceneNodeItem*> &vecItem = vecSceneNode[i]->GetChildItems();
            unsigned int taille2 = vecItem.size();
            for(unsigned int j=0;j<taille2;++j)
            {
                vecItem[j]->Draw(app);
            }
        }
    }
}
void SceneManager::ChangeNodeLevel(int ancien, int nouveau, SceneNode* noeud)
{
    std::vector<SceneNode*>& vec = m_nodesLevel[ancien];
    unsigned int taille = vec.size();
    for(unsigned int i=0;i<taille;++i)
    {
        if(vec[i]==noeud)
        {
            vec.erase(vec.begin()+i);
            break;
        }
    }
    m_nodesLevel[nouveau].push_back(noeud);
}
void SceneManager::AddNodeLevel(int level, SceneNode* node)
{
    m_nodesLevel[level].push_back(node);
}
void SceneManager::RemoveNode(SceneNode* node)
{
    int level = node->GetRealLevel();
    std::map<int,std::vector<SceneNode*>>::iterator it;
    it = m_nodesLevel.find(level);
    if(it!=m_nodesLevel.end())
    {
        std::vector<SceneNode*>::iterator it2;
        for(it2=it->second.begin();it2!=it->second.end();it2++)
        {
            if((*it2)==node)
            {
                std::vector<SceneNode*> nodes= (*it2)->GetChildSceneNodes(); // Ici on fait une copie, puisque cela est modifié dans la boucle, on évite de se couper l'herbe sous le pied...
                for(SceneNode* j : nodes)
                {
                    RemoveNode(j);
                }
                it->second.erase(it2);

                delete node;
                return;
            }
        }
    }
    else
    {
        Logger::Log()<<"Level not found !"<<Logger::endl;
    }
}

