/*
 * Collections-C
 * Copyright (C) 2013-2015 Srđan Panić <srdja.panic@gmail.com>
 *
 * This file is part of Collections-C.
 *
 * Collections-C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Collections-C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hashset.h"

struct hashset_s {
    HashTable *table;
    int *dummy;
};

/**
 * Returns a new HashTableProperties object that is by default configured for
 * string elements.
 *
 * @return a new HashTableProperties object.
 */
void hashset_conf_init(HashSetConf *conf)
{
    hashtable_conf_init(conf);
}

/**
 * Returns a new empty HashSet configured based on the specified properties
 * object.
 *
 * @param[in] properties the properties object used to configure the new HashSet
 *
 * @return a new empty HashSet
 */
HashSet *hashset_new()
{
    HashSetConf hsc;
    hashset_conf_init(&hsc);
    return hashset_new_conf(&hsc);
}

/**
 * Returns a new empty HashSet based on the specified HashSetConf object.
 *
 * The hashset is allocated using the allocators specified in the HashSetConf
 * object. The allocation may fail if the underlying allocator fails.
 *
 * @param[in] conf The hashset configuration object. All fields must be initialized.
 */
HashSet *hashset_new_conf(HashSetConf *conf)
{
    HashSet *set = calloc(1, sizeof(HashSet));
    set->table = hashtable_new_conf(conf);

    /* A dummy pointer that is never actually dereferenced
    *  that must not be null.*/
    set->dummy = (int*) 1;
    return set;
}

/**
 * Destroys the specified HashSet.
 *
 * @param[in] set the HashSet to be destroyed
 */
void hashset_destroy(HashSet *set)
{
    hashtable_destroy(set->table);
    free(set);
}

/**
 * Adds a new element to the set.
 *
 * @param[in] set the set to which the element is being added
 * @param[in] element the element being added
 */
void hashset_add(HashSet *set, void *element)
{
    hashtable_put(set->table, element, set->dummy);
}

/**
 * Removes and returns the specified element from the set if such an element
 * exists. This function returns NULL if the element was not a part of the
 * set.
 *
 * @param[in] set the set from which the element is being removed
 * @param[in] element the element being removed
 *
 * @return the removed element, or NULL if the element was not found
 */
void *hashset_remove(HashSet *set, void *element)
{
    /* Since the value is never null, a returned null can only
     * mean that the element is not present */
    return hashtable_remove(set->table, element) == NULL ? NULL : element;
}

/**
 * Removes all elemetns from the specified set.
 *
 * @param set the set from which all elements are being removed
 */
void hashset_remove_all(HashSet *set)
{
    hashtable_remove_all(set->table);
}

/**
 * Checks whether an element is a part of the specified set.
 *
 * @param[in] set the set being searched for the specified element
 * @parma[in] element the element being seached for
 *
 * @return true if the specified element is an element of the set
 */
bool hashset_contains(HashSet *set, void *element)
{
    return hashtable_contains_key(set->table, element);
}

/**
 * Returns the size of the specified set.
 *
 * @param[in] set the set whose size is being returned
 *
 * @return the size of the set
 */
size_t hashset_size(HashSet *set)
{
    return (size_t) hashtable_size(set->table);
}

/**
 * A 'foreach loop' function that invokes the specified function on every element
 * of the set. The operation function should not modify the elements. Any
 * modification of the elements will invalidate the set.
 *
 * @param[in] set the set on which this operation is being performed
 * @param[in] op the operation fuctnion that is invoked on each element of the
 *               set
 */
void hashset_foreach(HashSet *set, void (*op) (const void *e))
{
    hashtable_foreach_key(set->table, op);
}

/**
 * Initializes the iterator
 *
 * @param[in] iter the iterator that is being initialized
 * @param[in] set the set on which this iterator will operate
 */
void hashset_iter_init(HashSetIter *iter, HashSet *set)
{
    hashtable_iter_init(iter->iter, set->table);
}

/**
 * Checks whether or not there are more set elements to iterate over.
 *
 * @param[in] iter iterator on which this operation is being performed
 *
 * @return true if the iterator has not reached the end of the set
 */
bool hashset_iter_has_next(HashSetIter *iter)
{
    return hashtable_iter_has_next(iter->iter);
}

/**
 * Returns the next element in the sequence and advances the iterator.
 *
 * @param[in] iter the iterator that is being advanced
 *
 * @return the next element in the sequence
 */
const void *hashset_iter_next(HashSetIter *iter)
{
    TableEntry *entry = hashtable_iter_next(iter->iter);
    return (const void*) entry->key;
}

/**
 * Removes the last element returned by <code>hashset_iter_next()</code> and
 * returns that element.
 *
 * @param[in] iter the iterator from which the element is being removed
 *
 * @return the removed element
 */
void *hashset_iter_remove(HashSetIter *iter)
{
    void *element = iter->iter->prev_entry->key;
    hashtable_iter_remove(iter->iter);
    return element;
}
