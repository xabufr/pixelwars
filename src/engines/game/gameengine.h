#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include "../engine.h"
#include "carte.h"
#include "contactlistenner.h"
#include <deque>
#include "joueurmanager.h"
#include "unitemanager.h"
#include "inputmanager.h"

class JoueurHumain;
class Joueur;
class SceneNodeTextureItem;
class Projectile;
class TiXmlNode;
class GraphicalEngine;
class GuiWindowNode;
class GameEngine : public Engine
{
    public:
        GameEngine();
        virtual ~GameEngine();
        void Start();
        EngineType GetEngineId() const;
        bool CanWork() const;

        void Work();
        void HandleEngineEvent(EngineEvent*);

        void LoadGuiModels(GuiWindowNode*, Joueur*);
        void AddModel(GuiWindowContener*, Joueur*, TiXmlNode*);
        void DesalouerModel(GuiWindowContener*);
        void SetTailleMap(int);

        void SetSeed(int);

        void AddScore(int,int);

        static void CallbackAjoutUnite(GuiItem*);
        std::string GetWinner() const;

    private:
        ContactListenner *m_listner;
        void GererExplosions();
        void EnleverUnit(Unite*);
        void EnleverUnit(sf::Uint32);
        void ChangerScores();

        SceneNodeTextureItem* worldItem;
        b2World* m_world;
        b2Body* ground;
        Carte *m_carte;
        GraphicalEngine *m_gengine;

        JoueurManager *m_joueurManager;
        UniteManager *m_uniteManager;
        GuiWindowNode *m_windowJ1, *m_windowJ2;
        InputManager m_inputManager;
        sf::RenderWindow *m_app;
        GuiTextItem *m_scoreText[2];
        int m_score[2];

        int m_tailleCarte, m_seed;

        void DeleteProjectile(Projectile *toDelete);
        void SendEndMessage();

        bool m_running;

        sf::Uint32 m_lastId;
};

#endif // GAMEENGINE_H
