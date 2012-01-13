#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "scenenode.h"
#include <map>
#include <SFML2/Graphics.hpp>
/// @todo Documenter
class SceneManager
{
    friend class SceneNode;
    public:
        SceneManager();
        SceneNode* GetRootNode();
        virtual ~SceneManager();
        virtual void Draw();
        virtual void RemoveNode(SceneNode*);

    protected:
        virtual void ChangeNodeLevel(int,int,SceneNode*);//Appelé par les noeuds changeant de niveau
        virtual void AddNodeLevel(int, SceneNode*);
        SceneNode *m_rootSceneNode;
        std::map<int,std::vector<SceneNode*>> m_nodesLevel;
};

#endif // SCENEMANAGER_H
