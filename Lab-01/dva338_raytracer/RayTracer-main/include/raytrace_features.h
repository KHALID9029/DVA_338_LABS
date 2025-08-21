#ifndef _FEATURES_H_
#define _FEATURES_H_

/// To make it possible to easily reproduce different scenes from the assignment, 
/// you may use preprocessor features to turn on or off parts of your code at compile time
/// See more information in the example below or at
/// https://learn.microsoft.com/en-us/cpp/preprocessor/hash-if-hash-elif-hash-else-and-hash-endif-directives-c-cpp?view=msvc-170

/// For convenience, we have created a set of features for the base part of the assignment
/// along with the mapping between scenes and their respective feature sets.

/// First, we define the scene identifiers
/// The numbers map rougly to the section numbering in the assignment
/// e.g., render only ambient lighting is section 1.3.1 => 131
#define SCENE_AMBIENT_ONLY 131
#define SCENE_DIFFUSE_ONLY 132
#define SCENE_SPECULAR_ONLY 133
#define SCENE_FULL_LIGHTING 134
#define SCENE_SHADOWS_DARKNESS 140
#define SCENE_SHADOWS_AMBIENT 150
#define SCENE_REFLECTIONS 211
#define SCENE_TRIANGLES 220
#define SCENE_PERTURB_NORMALS 230

/// Then we define which scene we want to render
/// Replace the SCENE_* value with the scene you want to render from the values above
#define SCENE SCENE_PERTURB_NORMALS

// Finally, each scene will enable a set of features
#if SCENE == SCENE_AMBIENT_ONLY
#define AMBIENT_LIGHTING
#elif SCENE == SCENE_DIFFUSE_ONLY
#define DIFFUSE_LIGHTING
#elif SCENE == SCENE_SPECULAR_ONLY
#define SPECULAR_LIGHTING
#elif SCENE == SCENE_FULL_LIGHTING
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#elif SCENE == SCENE_SHADOWS_DARKNESS
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#define SHADOWS_BLACK
#elif SCENE == SCENE_SHADOWS_AMBIENT
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#define SHADOWS_AMBIENT
#elif SCENE == SCENE_REFLECTIONS
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#define SHADOWS_AMBIENT
#define REFLECTIONS
#elif SCENE == SCENE_TRIANGLES
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#define SHADOWS_AMBIENT
#define REFLECTIONS
#define TRIANGLES
#elif SCENE == SCENE_PERTURB_NORMALS
#define AMBIENT_LIGHTING
#define DIFFUSE_LIGHTING
#define SPECULAR_LIGHTING
#define SHADOWS_AMBIENT
#define REFLECTIONS
#define TRIANGLES
#define FUZZY_NORMALS
#endif
//Add Extras


/* 
Usage:
    Include this file in any project file that requires compilation-based control of features.
    If you wrap code related to the feature in an #if defined(FEATURE) block, 
    you can turn the feature on and off by changing the state of the define.
    
Example:   
    #if defined(AMBIENT_LIGHTING)
        col += ambient;
    #endif
    #if defined(DIFFUSE_LIGHTING)
        col += diffuse;
    #endif
    #if defined(SPECULAR_LIGHTING)
        col += specular;
    #endif
*/

#endif