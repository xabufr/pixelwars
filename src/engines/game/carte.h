#ifndef CARTE_H
#define CARTE_H
#include <Box2D/Box2D.h>
#include "engines/graphics/graphicalengine.h"
#include <deque>

class Carte
{
    public:
        Carte(b2World *world, const sf::Vector2i& taille, int seed);
        virtual ~Carte();
        void DemarrerDestruction();
        void FinirDestruction();
        void AjouterExplosion(sf::Vector2f position, float radius);
    protected:
    private:
        b2World *m_world;
        b2Body *m_bodyTerrain;
        SceneNodeTextureItem* m_itemTerrain;
        SceneNodeShapeItem* m_itemDessousTerrain;
        SceneNode* m_nodeTerrain;

        void RecalculerTerrain();

        void RecreerTerrain();
        void Fill(sf::Uint8 *toFill, int height);
        void Generer(const sf::Vector2i& taille, int seed);
        void CopierTableau(const int tab1[], int tab2[], const unsigned int Taille1);
        int DeterminerDeltaMin(int ordonnee, int listePts[], int nbPts);
        int DeterminerPotionVerticale(int x, int listePts[], int nbPts);

        void RegenererZone(int);
        void RegenererZonesModifiees();

        inline int DeterminerZone(int x);
        void AjouterZoneListeRegen(int);

        bool m_destruction, m_modified;
        sf::Vector2i m_taille;

        std::deque<std::deque<b2Fixture*>>m_fixturesMapZone;
        std::vector<int> m_zonesARegenrer;
        int m_tailleZone;
};

#endif // CARTE_H
