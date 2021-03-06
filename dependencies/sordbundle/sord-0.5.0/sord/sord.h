/*
  Copyright 2011 David Robillard <http://drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/**
   @file sord.h API for Sord, a lightweight RDF model library.
*/

#ifndef SORD_SORD_H
#define SORD_SORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "serd/serd.h"

#ifdef SORD_SHARED
#    ifdef __WIN32__
#        define SORD_LIB_IMPORT __declspec(dllimport)
#        define SORD_LIB_EXPORT __declspec(dllexport)
#    else
#        define SORD_LIB_IMPORT __attribute__((visibility("default")))
#        define SORD_LIB_EXPORT __attribute__((visibility("default")))
#    endif
#    ifdef SORD_INTERNAL
#        define SORD_API SORD_LIB_EXPORT
#    else
#        define SORD_API SORD_LIB_IMPORT
#    endif
#else
#    define SORD_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
   @defgroup sord Sord
   A lightweight RDF model library.

   Sord stores RDF (subject object predicate context) quads, where the context
   may be omitted (to represent triples in the default graph).
   @{
*/

/**
   Sord World.
   The World represents all library state, including interned strings.
*/
typedef struct SordWorldImpl SordWorld;

/**
   Sord Model.

   A model is an indexed set of Quads (i.e. it can contain several RDF
   graphs).  It may be searched using various patterns depending on which
   indices are enabled.
*/
typedef struct SordModelImpl SordModel;

/**
   Model Iterator.
*/
typedef struct SordIterImpl SordIter;

/**
   RDF Node.
   A Node is a component of a Quad.  Nodes may be URIs, blank nodes, or
   (in the case of quad objects only) string literals. Literal nodes may
   have an associate language or datatype (but not both).
*/
typedef struct SordNodeImpl SordNode;

/**
   Quad of nodes (a statement), or a quad pattern.

   Nodes are ordered (S P O G).  The ID of the default graph is 0.
*/
typedef const SordNode* SordQuad[4];

/**
   Index into a SordQuad.
*/
typedef enum {
	SORD_SUBJECT   = 0,  /**< Subject */
	SORD_PREDICATE = 1,  /**< Predicate (a.k.a. "key") */
	SORD_OBJECT    = 2,  /**< Object    (a.k.a. "value") */
	SORD_GRAPH     = 3   /**< Graph     (a.k.a. "context") */
} SordQuadIndex;

/**
   Type of a node.
*/
typedef enum {
	SORD_URI     = 1,  /**< URI */
	SORD_BLANK   = 2,  /**< Blank node identifier */
	SORD_LITERAL = 3   /**< Literal (string with optional lang or datatype) */
} SordNodeType;

/**
   Indexing option.
*/
typedef enum {
	SORD_SPO = 1,       /**< Subject,   Predicate, Object */
	SORD_SOP = 1 << 1,  /**< Subject,   Object,    Predicate */
	SORD_OPS = 1 << 2,  /**< Object,    Predicate, Subject */
	SORD_OSP = 1 << 3,  /**< Object,    Subject,   Predicate */
	SORD_PSO = 1 << 4,  /**< Predicate, Subject,   Object */
	SORD_POS = 1 << 5   /**< Predicate, Object,    Subject */
} SordIndexOption;

/**
   @name World
   @{
*/

/**
   Create a new Sord World.
   It is safe to use multiple worlds in one process, though no data
   (e.g. nodes) can be shared between worlds, and this should be avoided if
   possible for performance reasons.
*/
SORD_API
SordWorld*
sord_world_new(void);

/**
   Free @c world.
*/
SORD_API
void
sord_world_free(SordWorld* world);

/**
   @}
   @name Node
   @{
*/

/**
   Get a URI node from a string.

   Note this function measures @c str, which is a common bottleneck.
   Use sord_node_from_serd_node instead if @c str is already measured.
*/
SORD_API
SordNode*
sord_new_uri(SordWorld* world, const uint8_t* str);

/**
   Get a blank node from a string.

   Note this function measures @c str, which is a common bottleneck.
   Use sord_node_from_serd_node instead if @c str is already measured.
*/
SORD_API
SordNode*
sord_new_blank(SordWorld* world, const uint8_t* str);

/**
   Get a literal node from a string.

   Note this function measures @c str, which is a common bottleneck.
   Use sord_node_from_serd_node instead if @c str is already measured.
*/
SORD_API
SordNode*
sord_new_literal(SordWorld*     world,
                 SordNode*      datatype,
                 const uint8_t* str,
                 const char*    lang);

/**
   Copy a node (obtain a reference).

   Node that since nodes are interned and reference counted, this does not
   actually create a deep copy of @c node.
*/
SORD_API
SordNode*
sord_node_copy(const SordNode* node);

/**
   Free a node (drop a reference).
*/
SORD_API
void
sord_node_free(SordWorld* world, SordNode* node);

/**
   Return the type of a node (SORD_URI, SORD_BLANK, or SORD_LITERAL).
*/
SORD_API
SordNodeType
sord_node_get_type(const SordNode* node);

/**
   Return the string value of a node.
*/
SORD_API
const uint8_t*
sord_node_get_string(const SordNode* node);

/**
   Return the string value of a node, and set @c len to its length.
*/
SORD_API
const uint8_t*
sord_node_get_string_counted(const SordNode* node, size_t* len);

/**
   Return the language of a literal node (or NULL).
*/
SORD_API
const char*
sord_node_get_language(const SordNode* node);

/**
   Return the datatype URI of a literal node (or NULL).
*/
SORD_API
SordNode*
sord_node_get_datatype(const SordNode* node);

/**
   Return the flags (string attributes) of a node.
*/
SORD_API
SerdNodeFlags
sord_node_get_flags(const SordNode* node);

/**
   Return true iff node can be serialised as an inline object.

   More specifically, this returns true iff the node is the object field
   of exactly one statement, and therefore can be inlined since it needn't
   be referred to by name.
*/
SORD_API
bool
sord_node_is_inline_object(const SordNode* node);

/**
   Return true iff @c a is equal to @c b.

   Note this is much faster than comparing the node's strings.
*/
SORD_API
bool
sord_node_equals(const SordNode* a,
                 const SordNode* b);

/**
   Return a SordNode as a SerdNode.

   The returned node is shared and must not be freed or modified.
*/
SORD_API
const SerdNode*
sord_node_to_serd_node(const SordNode* node);

/**
   Create a new SordNode from a SerdNode.

   The returned node must be freed using sord_node_free.
*/
SORD_API
SordNode*
sord_node_from_serd_node(SordWorld*      world,
                         SerdEnv*        env,
                         const SerdNode* node,
                         const SerdNode* datatype,
                         const SerdNode* lang);

/**
   @}
   @name Model
   @{
*/

/**
   Create a new model.

   @param world The world in which to make this model.

   @param indices SordIndexOption flags (e.g. SORD_SPO|SORD_OPS).  Be sure to
   enable an index where the most significant node(s) are not variables in your
   queries (e.g. to make (? P O) queries, enable either SORD_OPS or SORD_POS).

   @param graphs If true, store (and index) graph contexts.
*/
SORD_API
SordModel*
sord_new(SordWorld* world,
         unsigned  indices,
         bool      graphs);

/**
   Close and free @c model.
*/
SORD_API
void
sord_free(SordModel* model);

/**
   Get the world associated with @c model.
*/
SORD_API
SordWorld*
sord_get_world(SordModel* model);

/**
   Return the number of nodes stored in @c world.

   Nodes are included in this count iff they are a part of a quad in @c world.
*/
SORD_API
size_t
sord_num_nodes(const SordWorld* world);

/**
   Return the number of quads stored in @c model.
*/
SORD_API
size_t
sord_num_quads(const SordModel* model);

/**
   Return an iterator to the start of @c model.
*/
SORD_API
SordIter*
sord_begin(const SordModel* model);

/**
   Search for a triple pattern.
   @return an iterator to the first match, or NULL if no matches found.
*/
SORD_API
SordIter*
sord_find(SordModel* model, const SordQuad pat);

/**
   Check if @a model contains a triple pattern.
*/
SORD_API
bool
sord_contains(SordModel* model, const SordQuad pat);

/**
   Add a quad to a model.
*/
SORD_API
bool
sord_add(SordModel* model, const SordQuad quad);

/**
   Remove a quad from a model.

   Note that is it illegal to remove while iterating over @c model.
*/
SORD_API
void
sord_remove(SordModel* model, const SordQuad quad);

/**
   @}
   @name Iteration
   @{
*/

/**
   Set @c quad to the quad pointed to by @c iter.
*/
SORD_API
void
sord_iter_get(const SordIter* iter, SordQuad quad);

/**
   Return the store pointed to by @c iter.
*/
SORD_API
const SordModel*
sord_iter_get_model(SordIter* iter);

/**
   Increment @c iter to point to the next statement.
*/
SORD_API
bool
sord_iter_next(SordIter* iter);

/**
   Return true iff @c iter is at the end of its range.
*/
SORD_API
bool
sord_iter_end(const SordIter* iter);

/**
   Free @c iter.
*/
SORD_API
void
sord_iter_free(SordIter* iter);

/**
   @}
   @name Utilities
   @{
*/

/**
   Match two quads (using ID comparison only).

   This function is a straightforward and fast equivalence match with wildcard
   support (ID 0 is a wildcard). It does not actually read node data.
   @return true iff @c x and @c y match.
*/
SORD_API
bool
sord_quad_match(const SordQuad x, const SordQuad y);

/**
   @}
   @name Serialisation
   @{
*/

/**
   Return a reader that will read into @c model.
*/
SORD_API
SerdReader*
sord_new_reader(SordModel* model,
                SerdEnv*   env,
                SerdSyntax syntax,
                SordNode*  graph);

/**
   Write a model to a writer.
*/
SORD_API
bool
sord_write(SordModel*  model,
           SerdWriter* writer,
           SordNode*   graph);

/**
   Write a range to a writer.

   This increments @c iter to its end, then frees it.
*/
SORD_API
bool
sord_write_iter(SordIter*   iter,
                SerdWriter* writer);

/**
   @}
   @}
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* SORD_SORD_H */
