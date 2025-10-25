#include "Projectile.hpp"
#include "TileNode.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "PlayerData.hpp"

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder &textures, Tilemap &tilemap, Rotation rotation, PlayerData *playerData, int &bullets) :
    Entity(1),
    mType(type),
    mSprite(textures.get(Textures::BulletTileset)),
    mTilemap(tilemap),
    mRotation(rotation),
    mGrade(playerData->getGrade()),
    mPlayerData(playerData),
    mBullets(bullets),
    mVelocity(),
    mExplosion(textures.get(Textures::SmallExplosionAnimation), sf::Vector2i(16, 16), 2, sf::seconds(8.f / 60.f)),
    mShowExplosion(true)
{
    sf::Vector2i bulletSize{ 4, 4 };
    sf::Vector2f explosionOffset;
    if (mRotation == Rotation::Left || mRotation == Rotation::Right)
    {
        explosionOffset.x += mRotation == Rotation::Left ? -8 : 0;
        explosionOffset.y -= 6;
        mSprite.setTextureRect(mRotation == Rotation::Left ? sf::IntRect({ 0, 0 }, bulletSize) : sf::IntRect({ 4, 0 }, bulletSize));
        mVelocity.x += mRotation == Rotation::Left ? -Table[static_cast<unsigned int>(type)].pixelPerFrame : Table[static_cast<unsigned int>(type)].pixelPerFrame;
    }
    else if (mRotation == Rotation::Up || mRotation == Rotation::Down)
    {
        explosionOffset.x -= 6;
        explosionOffset.y += mRotation == Rotation::Up ? -8 : 0;
        mSprite.setTextureRect(mRotation == Rotation::Up ? sf::IntRect({ 8, 0 }, bulletSize) : sf::IntRect({ 12, 0 }, bulletSize));
        mVelocity.y += mRotation == Rotation::Up ? -Table[static_cast<unsigned int>(type)].pixelPerFrame : Table[static_cast<unsigned int>(type)].pixelPerFrame;
    }
    mExplosion.move(explosionOffset);
}

Projectile::Projectile(Type type, const TextureHolder &textures, Tilemap &tilemap, Rotation rotation, int &bullets) :
    Entity(1),
    mType(type),
    mSprite(textures.get(Textures::BulletTileset)),
    mTilemap(tilemap),
    mRotation(rotation),
    mGrade(0),
    mPlayerData(nullptr),
    mBullets(bullets),
    mVelocity(),
    mExplosion(textures.get(Textures::SmallExplosionAnimation), sf::Vector2i(16, 16), 2, sf::seconds(8.f / 60.f)),
    mShowExplosion(true)
{
    sf::Vector2i bulletSize{ 4, 4 };
    sf::Vector2f explosionOffset;
    if (mRotation == Rotation::Left || mRotation == Rotation::Right)
    {
        explosionOffset.x += mRotation == Rotation::Left ? -8 : 0;
        explosionOffset.y -= 6;
        mSprite.setTextureRect(mRotation == Rotation::Left ? sf::IntRect({ 0, 0 }, bulletSize) : sf::IntRect({ 4, 0 }, bulletSize));
        mVelocity.x += mRotation == Rotation::Left ? -Table[static_cast<unsigned int>(type)].pixelPerFrame : Table[static_cast<unsigned int>(type)].pixelPerFrame;
    }
    else if (mRotation == Rotation::Up || mRotation == Rotation::Down)
    {
        explosionOffset.x -= 6;
        explosionOffset.y += mRotation == Rotation::Up ? -8 : 0;
        mSprite.setTextureRect(mRotation == Rotation::Up ? sf::IntRect({ 8, 0 }, bulletSize) : sf::IntRect({ 12, 0 }, bulletSize));
        mVelocity.y += mRotation == Rotation::Up ? -Table[static_cast<unsigned int>(type)].pixelPerFrame : Table[static_cast<unsigned int>(type)].pixelPerFrame;
    }
    mExplosion.move(explosionOffset);
}

Category Projectile::getCategory() const
{
    switch (mType)
    {
        case Projectile::Type::FirstPlayerSlowBullet:
        case Projectile::Type::FirstPlayerFastBullet:
            return Category::FirstPlayerProjectile;
        case Projectile::Type::SecondPlayerSlowBullet:
        case Projectile::Type::SecondPlayerFastBullet:
            return Category::SecondPlayerProjectile;
        default:
            return Category::EnemyProjectile;
    }
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Projectile::isMarkedForRemoval() const
{
    return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Projectile::remove()
{
    mBullets += 1;
    Entity::destroy();
    mShowExplosion = false;
}

void Projectile::destroy()
{
    mBullets += 1;
    Entity::destroy();
}

int Projectile::getDamage() const
{
    return Table[static_cast<unsigned int>(mType)].damage;
}

PlayerData *Projectile::getPlayerData()
{
    return mPlayerData;
}

void Projectile::updateCurrent(sf::Time deltaTime, CommandQueue &commands)
{
    if (!isDestroyed())
    {
        checkNextPosition(mVelocity) ? move(mVelocity) : destroy();
    }
    else
    {
        mExplosion.update(deltaTime);
    }
}

void Projectile::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!isDestroyed())
    {
        target.draw(mSprite, states);
    }
    else if (mShowExplosion)
    {
        target.draw(mExplosion, states);
    }
}

bool Projectile::checkNextPosition(sf::Vector2f velocity)
{
    sf::FloatRect nextPosition = getBoundingRect();
    nextPosition.position += velocity;
    bool intersected = false;
    for (auto &tile : mTilemap.get())
    {
        Tilemap::Type &tileNumber = tile.tileNumber;
        if (tileNumber == Tilemap::Air || tileNumber == Tilemap::Leaf || tileNumber == Tilemap::Ice ||
            tileNumber == Tilemap::Water || tileNumber == Tilemap::DestroyedFirstHalfOfBase || tileNumber == Tilemap::DestroyedSecondHalfOfBase)
        {
            continue;
        }
        Tilemap::Modifier &modifier = tile.modifier;
        sf::Vector2f backgroundOffset{ 16, 8 };
        sf::Vector2f tilePosition{ tile.x * 8.f, tile.y * 8.f };
        tilePosition += backgroundOffset;
        sf::Vector2f tileSize{ 8, 8 };
        if (tileNumber == Tilemap::Brick && modifier != Tilemap::Full)
        {
            setBrickModifierTile(tilePosition, tileSize, modifier);
        }
        sf::FloatRect tileRect(tilePosition, tileSize);
        if (tileRect.findIntersection(nextPosition))
        {
            if (tileNumber == Tilemap::Brick)
            {
                if (mGrade > 2)
                {
                    tile.updateTile(Tilemap::Air);
                }
                else if (modifier == Tilemap::BrickFull)
                {
                    if (mRotation == Rotation::Left)
                    {
                        modifier = Tilemap::BrickLeft;
                    }
                    else if (mRotation == Rotation::Right)
                    {
                        modifier = Tilemap::BrickRight;
                    }
                    else if (mRotation == Rotation::Up)
                    {
                        modifier = Tilemap::BrickUp;
                    }
                    else
                    {
                        modifier = Tilemap::BrickDown;
                    }
                }
                else if (modifier == Tilemap::BrickLeft)
                {
                    if (mRotation == Rotation::Up || mRotation == Rotation::Down)
                    {
                        modifier = mRotation == Rotation::Up ? Tilemap::BrickLeftUpperQuarter : Tilemap::BrickLeftLowerQuarter;
                    }
                    else
                    {
                        tile.updateTile(Tilemap::Air);
                    }
                }
                else if (modifier == Tilemap::BrickRight)
                {
                    if (mRotation == Rotation::Up || mRotation == Rotation::Down)
                    {
                        modifier = mRotation == Rotation::Up ? Tilemap::BrickRightUpperQuarter : Tilemap::BrickRightLowerQuarter;
                    }
                    else
                    {
                        tile.updateTile(Tilemap::Air);
                    }
                }
                else if (modifier == Tilemap::BrickUp)
                {
                    if (mRotation == Rotation::Left || mRotation == Rotation::Right)
                    {
                        modifier = mRotation == Rotation::Left ? Tilemap::BrickLeftUpperQuarter : Tilemap::BrickRightUpperQuarter;
                    }
                    else
                    {
                        tile.updateTile(Tilemap::Air);
                    }
                }
                else if (modifier == Tilemap::BrickDown)
                {
                    if (mRotation == Rotation::Left || mRotation == Rotation::Right)
                    {
                        modifier = mRotation == Rotation::Left ? Tilemap::BrickLeftLowerQuarter : Tilemap::BrickRightLowerQuarter;
                    }
                    else
                    {
                        tile.updateTile(Tilemap::Air);
                    }
                }
                else if (modifier == Tilemap::BrickLeftUpperQuarter || modifier == Tilemap::BrickLeftLowerQuarter ||
                         modifier == Tilemap::BrickRightUpperQuarter || modifier == Tilemap::BrickRightLowerQuarter)
                {
                    tile.updateTile(Tilemap::Air);
                }
                intersected = true;
            }
            else if (tileNumber == Tilemap::Concrete)
            {
                if (mGrade > 2)
                {
                    tile.updateTile(Tilemap::Air);
                }
                intersected = true;
            }
            else if (tileNumber == Tilemap::FirstHalfOfBase || tileNumber == Tilemap::SecondHalfOfBase)
            {
                mTilemap.destroyBase();
                intersected = true;
            }
            else if (tileNumber == Tilemap::DestroyedFirstHalfOfBase || tileNumber == Tilemap::DestroyedSecondHalfOfBase)
            {
                intersected = true;
            }
            else
            {
                throw std::logic_error("Unexcepted tile.");
            }
        }
    }
    if (intersected)
    {
        return false;
    }
    if (16 <= nextPosition.position.x && nextPosition.position.x + nextPosition.size.x <= 224 && 8 <= nextPosition.position.y && nextPosition.position.y + nextPosition.size.y <= 216)
    {
        return true;
    }
    return false;
}

void Projectile::setBrickModifierTile(sf::Vector2f &position, sf::Vector2f &size, Tilemap::Modifier modifier)
{
    if (modifier == Tilemap::BrickLeft)
    {
        size.x -= 4;
    }
    else if (modifier == Tilemap::BrickRight)
    {
        size.x -= 4;
        position.x += 4;
    }
    else if (modifier == Tilemap::BrickUp)
    {
        size.y -= 4;
    }
    else if (modifier == Tilemap::BrickDown)
    {
        size.y -= 4;
        position.y += 4;
    }
    else if (modifier == Tilemap::BrickLeftUpperQuarter)
    {
        size.x -= 4;
        size.y -= 4;
    }
    else if (modifier == Tilemap::BrickLeftLowerQuarter)
    {
        size.x -= 4;
        size.y -= 4;
        position.y += 4;
    }
    else if (modifier == Tilemap::BrickRightUpperQuarter)
    {
        size.x -= 4;
        size.y -= 4;
        position.x += 4;
    }
    else if (modifier == Tilemap::BrickRightLowerQuarter)
    {
        size.x -= 4;
        size.y -= 4;
        position.x += 4;
        position.y += 4;
    }
    else
    {
        throw std::logic_error("Unexcepted modifier of tile brick.");
    }
}