#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "glm.hpp"


class SpatialHashGrid;
class ObjectManager;
class Camera2D;
class RenderManager;
class Object;
class CircleCollider;
class AABBCollider;

enum class ColliderType
{
    None,
    Circle,
    AABB
};

class Collider
{
    friend ObjectManager;
    friend CircleCollider;
    friend AABBCollider;
    friend SpatialHashGrid;
public:
    Collider() = delete;
    Collider(Object* owner_) : owner(owner_), offset(){}
    virtual ~Collider() = default;

    void SetUseTransformScale(bool use) { useTransformScale = use; }
    [[nodiscard]] bool IsUsingTransformScale() const { return useTransformScale; }

    void SetOffset(const glm::vec2& pos) { offset = pos; }
    [[nodiscard]] const glm::vec2& GetOffset() const { return offset; }

    [[nodiscard]] virtual bool CheckPointCollision(const glm::vec2& point) const = 0;

protected:
    [[nodiscard]] Object* GetOwner() const { return owner; }

    [[nodiscard]] virtual ColliderType GetType() const = 0;

    [[nodiscard]] virtual float GetBoundingRadius() const = 0;

    [[nodiscard]] virtual bool CheckCollision(const Collider* other) const = 0;

    [[nodiscard]] virtual bool DispatchAgainst(const CircleCollider& other) const = 0;
    [[nodiscard]] virtual bool DispatchAgainst(const AABBCollider& other) const = 0;

    virtual void SyncWithTransformScale() = 0;

    virtual void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color = { 1,0,0,1 }) const = 0;

    Object* owner;
    bool useTransformScale = true;
    glm::vec2 offset;
};


class CircleCollider : public Collider
{
    friend AABBCollider;
    friend SpatialHashGrid;
public:
    CircleCollider(Object* owner, float size)
        : Collider(owner), baseRadius(size/2.f), scaledRadius(size/2.f) {
    }

    [[nodiscard]] float GetRadius() const;

    [[nodiscard]] float GetSize() const;

    void SetRadius(float r);

    [[nodiscard]] bool CheckPointCollision(const glm::vec2& point) const override;

private:
    [[nodiscard]] ColliderType GetType() const override { return ColliderType::Circle; }

    [[nodiscard]] float GetBoundingRadius() const override;

    [[nodiscard]] bool CheckCollision(const Collider* other) const override;
    
    [[nodiscard]] bool DispatchAgainst(const CircleCollider& other) const override;
    [[nodiscard]] bool DispatchAgainst(const AABBCollider& other) const override;

    void SyncWithTransformScale() override;

    void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const override;

    float baseRadius = 0.5f;
    float scaledRadius = 0.5f;
};


class AABBCollider : public Collider
{
    friend CircleCollider;
    friend SpatialHashGrid;
public:
    AABBCollider(Object* owner, const glm::vec2& size)
        : Collider(owner), baseHalfSize(size/glm::vec2(2)), scaledHalfSize(size / glm::vec2(2)) {
    }

    [[nodiscard]] glm::vec2 GetHalfSize() const;

    [[nodiscard]] glm::vec2 GetSize() const;
    void SetSize(const glm::vec2& hs);

    [[nodiscard]] bool CheckPointCollision(const glm::vec2& point) const override;

private:
    [[nodiscard]] ColliderType GetType() const override { return ColliderType::AABB; }

    [[nodiscard]] float GetBoundingRadius() const override;

    [[nodiscard]] bool CheckCollision(const Collider* other) const override;

    [[nodiscard]] bool DispatchAgainst(const CircleCollider& other) const override;
    [[nodiscard]] bool DispatchAgainst(const AABBCollider& other) const override;

    void SyncWithTransformScale() override;

    void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const override;

    glm::vec2 baseHalfSize = { 0.5f, 0.5f };
    glm::vec2 scaledHalfSize = { 0.5f, 0.5f };
};

struct Vec2Hash
{
    size_t operator()(const glm::ivec2& v) const
    {
        return std::hash<int>()(v.x * 73856093 ^ v.y * 19349663);
    }
};

class SpatialHashGrid
{
    friend ObjectManager;
private:
    void Clear();
    void Insert(Object* obj);
    void ComputeCollisions(std::function<void(Object*, Object*)> onCollision);
    [[nodiscard]] glm::ivec2 GetCell(const glm::vec2& pos) const;
    void InsertToCell(Object* obj, const glm::ivec2& cell);

    int cellSize = 50; 
    std::unordered_map<glm::ivec2, std::vector<Object*>, Vec2Hash> grid;
    std::vector<Object*> largeObjects;
    std::vector<Object*> objects;
};

class CollisionGroupRegistry
{
    friend Collider;
    friend Object;
private:
    [[nodiscard]] uint32_t GetGroupBit(const std::string& tag);
    [[nodiscard]] std::string GetGroupTag(uint32_t bit) const;
    std::unordered_map<std::string, uint32_t> tagToBit;
    std::unordered_map<uint32_t, std::string> bitToTag;
    uint32_t currentBit = 0;
};