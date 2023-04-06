#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "functions.h"

#define FPS 60

double global_to_lcoord(vector point, ray a) {  // transform global coordinate to local ray coordinate (doesn't check if the point is actually on the line!)
    return (point + a.m_pos * -1)[0] / a.m_vers[0];
}

vector collide(ray a, surface b) {  // compute ray-surfafe intersection (coordinates in ray and surface reference frame)
    double ma[] = {
        a.m_vers[0], -b.m_v1[0], -b.m_v2[0],
        a.m_vers[1], -b.m_v1[1], -b.m_v2[1],
        a.m_vers[2], -b.m_v1[2], -b.m_v2[2],
    };
    matrix m(3, ma);

    if (m.det() == 0) {  // matrix is singular --> surface isn't valid or is parallel to ray
        double tarr[] = {NAN};
        return vector(1, tarr);
    }

    m = m.inverse();
    vector t(3);
    t = m * (b.m_pos + (a.m_pos * -1)); // solve linear system (ray coord, surf coord 1, surf coord 2)

    // check if point is in triangle
    vector p = a(t[0]);
    vector p0 = b.m_pos;
    vector r = p + p0 * -1;
    vector v3 = b.m_v1 + b.m_v2 * -1;
    ray test_line(p, r);    ray tri_side(p0 + b.m_v2, v3);
    vector p1 = intersect(test_line, tri_side); // cast a ray passing for the intersection of surf.v1 and surf.v2. find the intersection with <v3>
    if (global_to_lcoord(p1, tri_side) >= 0 and global_to_lcoord(p1, tri_side) <= 1 ) { // check if ray intersects the side of the triangle, not just its extension
        if ((global_to_lcoord(p1, test_line) >= 0))    // if the coordinate is positive the point is in the triangle
            return a(t[0]); // local coord to global
    }

    double tarr[] = {NAN};
    return vector(1, tarr);

}

class camera {
    public:

    camera(vector pos, vector dptof, vector width, vector height) {
        m_pos = pos;
        m_dptof = dptof;
        m_width = width;
        m_height = height;

    }

    SDL_Surface render(surface* surf_arr, int surf_count, int max_bounces, int* res) {

        for (int ix=0; ix<res[0]; ix++) {   // loop trough columns
            for (int iy=0; iy<res[1]; iy++) {   // loop trough rows
                vector pix_pos = m_pos + m_dptof +  m_width * ((m_width.norm() / res[0]) * ix - 0.5) + m_height * ((m_height.norm() / res[1]) * iy - 0.5);

                ray r(m_pos, (pix_pos + m_pos * -1) * (1 / (pix_pos + m_pos * -1).norm()));

                for (int bounce=0; bounce < max_bounces; bounce++) {
                    double min_dist = -1;
                    int bounce_ind = -1;
                    for (int i=0; i<surf_count; i++) {
                        vector coll_point = collide(r, surf_arr[i]);

                        if (isnan(coll_point[0]))    // if there is no collision, skip this surface
                            continue;

                        double dist = global_to_lcoord(coll_point, r);   // the distance covered by the ray is the coordinate of the collision with respect to itself

                        if (dist <0)
                            continue;

                        if (dist < min_dist or min_dist < 0) {
                            min_dist = dist;
                            bounce_ind = i;
                        }
                    }

                    vector new_pos = r(min_dist);   // the new ray starts at the (first) collision point'

                    // compute new ray direction
                    vector normal = cross_prod(surf_arr[bounce_ind].m_v1, surf_arr[bounce_ind].m_v2);   // compute surface normal
                    normal = normal * (1/normal.norm());    // normalization
                    vector axis = cross_prod(r.m_vers, normal); // compute rotation axis
                    axis = axis * (1/axis.norm());  // normalization
                    double theta = angle(r.m_vers, normal);
                    matrix rotation = rot(axis, 2 * theta);
                    vector new_vers = rotation * r.m_vers;  // rotate the old versor by 2 * the angle between the versor and the normal, around axis

                    // update ray r with the new ray info
                    r.m_vers = new_vers;
                    r.m_pos = new_pos;

                    // There is no exit condition for hitting a light source!!

                }


            }

        }
    }

    vector m_pos;   // observer position
    vector m_dptof; // depth of field and orientation
    vector m_width; // screen width and horizontal orientation of the screen
    vector m_height;    // screen height and vertical orientation of the screen
};

int main() {
    app_data app;
    init(&app, 1000, 500);

    while (true) {
        Uint32 t0 = SDL_GetTicks();

        event_handle();

        Uint32 t1 = SDL_GetTicks();
        int delay = 1000/FPS - (t1-t0);
        SDL_RenderPresent(app.renderer);
        if (delay > 0) SDL_Delay(delay);
        
        //break; // debug
    }

    return 0;
}
