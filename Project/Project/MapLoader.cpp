#include "MapLoader.h"
#include <fstream>
#include <vector>
#include "GameObject.h"

// free space initizilation
std::vector<glm::vec2> MapLoader::freeSpaces;

bool MapLoader::Load(const std::string& filename, const EngineContext& context)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false; 
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    if (lines.empty()) return false;

    int totalRows = lines.size();
    int totalCols = lines[0].length();

    float screenW = context.windowManager->GetWidth();
    float screenH = context.windowManager->GetHeight();

    float tileW = screenW / totalCols;
    float tileH = screenH / totalRows;

    float tileSize = std::min(tileW, tileH);
    float wallScale = tileSize;

    float startX = -(totalCols * tileSize) / 2.0f + (tileSize / 2.0f);
    float startY = (totalRows * tileSize) / 2.0f - (tileSize / 2.0f);

    ObjectManager& objManager = context.stateManager->GetCurrentState()->GetObjectManager();
    freeSpaces.clear(); 

    for (int row = 0; row < totalRows; row++)
    {
        for (int col = 0; col < lines[row].length(); col++)
        {
            char c = lines[row][col];
            glm::vec2 pos(startX + (col * tileSize), startY - (row * tileSize));

            if (c == '*')
            {
                GameObject* block = static_cast<GameObject*>(objManager.AddObject(std::make_unique<GameObject>(), "[Object]Wall"));
                block->SetMesh(context, "[EngineMesh]default");
                block->SetMaterial(context, "[Material]StoneWall");
                block->GetTransform2D().SetScale({ wallScale, wallScale });
                block->SetRenderLayer("[Layer]Items");
                block->GetTransform2D().SetPosition(pos);

                block->SetCollider(std::make_unique<AABBCollider>(block, glm::vec2{ wallScale, wallScale }));
                block->GetCollider()->SetUseTransformScale(false);
                block->SetCollision(objManager, "[Object]Wall", { "[Object]Player1", "[Object]Player2", "[Object]Bullet" });
            }

            else if (c == '.')
            {
                freeSpaces.push_back(pos);
            }
        }
    }

    return true; 
}