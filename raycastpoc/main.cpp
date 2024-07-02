#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767837449448255379774726847104047534646208046684259069491293313677028989152104752162056966024058038150193511253382430035587640247496473263914199272604269922796782354781636009341721641219924586315030286182974555706749838505494588586926995690927210797509302955321165344987202755960236480665499119881834797753566369807426542527862551818417574672890977772793800081647060016145249192173217214772350141441973568548161361157352552133475741849468438523323907394143334547762416862518983569485562099219222184272550254256887671790494601653466804988627232791786085784383827967976681454100953883786360950680064225125205117392984896084128488626945604241965285022210661186306744278622039194945047123713786960956364371917287467764657573962413890865832645995813390478027590099465764078951269468398352595709825822620522
// tem 2043 casas decimais aqui, quase o limite do compilador xddd

struct Point3D {
    float x, y, z;
};

struct AABB {
    Point3D min;
    Point3D max;
};

bool intervalsIntersect(float minA, float maxA, float minB, float maxB) {
    return maxA >= minB && maxB >= minA;
}

bool lineIntersectsAABB(const Point3D& p1, const Point3D& p2, const AABB& aabb) {
    float tmin = (aabb.min.x - p1.x) / (p2.x - p1.x);
    float tmax = (aabb.max.x - p1.x) / (p2.x - p1.x);

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (aabb.min.y - p1.y) / (p2.y - p1.y);
    float tymax = (aabb.max.y - p1.y) / (p2.y - p1.y);

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (aabb.min.z - p1.z) / (p2.z - p1.z);
    float tzmax = (aabb.max.z - p1.z) / (p2.z - p1.z);

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return tmin < 1.0f && tmax > 0.0f;
}

void drawScene(const Point3D& local, const AABB& wall, const Point3D& enemy, bool isVisible) {
    for (int z = 0; z < 20; ++z) {
        for (int x = 0; x < 60; ++x) {
            if (static_cast<int>(x) == static_cast<int>(local.x) && static_cast<int>(z) == static_cast<int>(local.z)) {
                std::cout << 'A';
            }
            else if (x >= static_cast<int>(wall.min.x) && x <= static_cast<int>(wall.max.x) && z >= static_cast<int>(wall.min.z) && z <= static_cast<int>(wall.max.z)) {
                std::cout << '#';
            }
            else if (static_cast<int>(x) == static_cast<int>(enemy.x) && static_cast<int>(z) == static_cast<int>(enemy.z)) {
                std::cout << (isVisible ? 'E' : 'e');
            }
            else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    Point3D local = { 5.0f, 10.0f, 5.0f };
    AABB wall = { {20.0f, 10.0f, 7.0f}, {40.0f, 10.0f, 13.0f} };
    Point3D enemy = { 10.0f, 10.0f, 15.0f };

    float angle = 0.0f;
    float radius = 15.0f;
    float centerX = (wall.min.x + wall.max.x) / 2.0f;
    float centerZ = (wall.min.z + wall.max.z) / 2.0f;

    while (true) {
        system("cls");

        bool isVisible = !lineIntersectsAABB(local, enemy, wall);
        drawScene(local, wall, enemy, isVisible);

        angle += 0.1f;
        if (angle >= 2 * M_PI) {
            angle -= 2 * M_PI;
        }

        enemy.x = centerX + radius * std::cos(angle) + static_cast<float>(std::rand() % 3 - 1) * 0.1f;
        enemy.z = centerZ + radius * std::sin(angle) + static_cast<float>(std::rand() % 3 - 1) * 0.1f;

        if (enemy.x < 0.0f) enemy.x = 0.0f;
        if (enemy.x > 59.0f) enemy.x = 59.0f;
        if (enemy.z < 0.0f) enemy.z = 0.0f;
        if (enemy.z > 19.0f) enemy.z = 19.0f;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
