#include "../hash_table.h"
#include "../common.h"
int extract_key(elem_t key)
{
  return key.int_val;
}

char *extract_str(elem_t value)
{
  return value.str_val;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, cmp_func_t key_cmp_func, cmp_func_t val_cmp_func)
{
  ioopm_hash_table_t *table = calloc(1, sizeof(ioopm_hash_table_t));
  if (hash_func != NULL)
  {
    table->hash_func = hash_func;
  }
  else
  {
    table->hash_func = extract_key;
  }
  table->key_cmp_func = key_cmp_func;
  table->val_cmp_func = val_cmp_func;
  return table;
}

void destroy_entries(ioopm_entry_t *entry)
{
  if (entry == NULL)
  {
    return;
  }
  else
  {
    destroy_entries(entry->next);
    free(entry);
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  free(ht);
}

static ioopm_entry_t *entry_create(elem_t key, elem_t value, ioopm_entry_t *prev_entry)
{
  ioopm_entry_t *new_entry = (ioopm_entry_t *)calloc(1, sizeof(ioopm_entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = prev_entry;
  return new_entry;
}

static ioopm_entry_t *find_previous_entry_for_key(ioopm_entry_t *entry, elem_t key, cmp_func_t key_cmp)
{
  ioopm_entry_t *current_entry = entry;
  while (current_entry->next != NULL && key_cmp(current_entry->next->key, key) != 0)
  {
    current_entry = current_entry->next;
  }
  return current_entry;
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{

  int bucket = abs(ht->hash_func(key)) % NO_BUCKETS;
  ioopm_entry_t *prev_entry = find_previous_entry_for_key(&(ht->buckets[bucket]), key, ht->key_cmp_func);
  ioopm_entry_t *current_entry = prev_entry->next;
  elem_t removed_value;

  if (current_entry && ht->key_cmp_func(current_entry->key, key) == 0)
  {
    // Check if entry is the last
    if (!current_entry->next)
    {
      prev_entry->next = NULL;
    }
    else
    {
      prev_entry->next = current_entry->next;
    }
    removed_value = current_entry->value;
    free(current_entry);
    return Success(removed_value);
  }
  return Failure();
}
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{

  int bucket = abs(ht->hash_func(key)) % NO_BUCKETS;
  ioopm_entry_t *prev_entry = find_previous_entry_for_key(&(ht->buckets[bucket]), key, ht->key_cmp_func);
  ioopm_entry_t *current_entry = prev_entry->next;

  if (current_entry && ht->key_cmp_func(current_entry->key, key) == 0)
  {
    current_entry->value = value;
  }
  else
  {
    prev_entry->next = entry_create(key, value, current_entry);
  }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = abs(ht->hash_func(key)) % NO_BUCKETS;
  ioopm_entry_t *prev_entry = find_previous_entry_for_key(&(ht->buckets[bucket]), key, ht->key_cmp_func);
  ioopm_entry_t *current_entry = prev_entry->next;

  if (current_entry && ht->key_cmp_func(current_entry->key, key) == 0)
  {
    return Success(current_entry->value);
  }
  else
  {
    return Failure();
  }
}

size_t bucket_size(ioopm_entry_t *entry)
{
  if (entry == NULL)
  {
    return 0;
  }
  return bucket_size(entry->next) + 1;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t size = 0;
  for (int i = 0; i < NO_BUCKETS; i++)
  {
    size += bucket_size(ht->buckets[i].next);
  }

  return size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < NO_BUCKETS; i++)
  {
    if ((ht->buckets[i]).next != NULL)
      return false;
  }
  return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < NO_BUCKETS; i++)
  {
    if (!ht->buckets[i].next)
      continue;
    destroy_entries(ht->buckets[i].next); // -> next ska tas bort
    ht->buckets[i].next = NULL;
  }
}

ioopm_linked_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_linked_list_t *all_keys = ioopm_linked_list_create(ht->key_cmp_func);

  for (int i = 0; i < NO_BUCKETS; i++)
  {
    if (ht->buckets[i].next != NULL)
    {
      ioopm_entry_t *entry = ht->buckets[i].next;

      while (entry != NULL)
      {
        ioopm_linked_list_append(all_keys, entry->key);
        entry = entry->next;
      }
    }
  }
  return all_keys;
}

ioopm_linked_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_linked_list_t *all_values = ioopm_linked_list_create(ht->val_cmp_func);

  for (int i = 0; i < NO_BUCKETS; i++)
  {
    if (ht->buckets[i].next != NULL)
    {
      ioopm_entry_t *entry = ht->buckets[i].next;

      while (entry != NULL)
      {
        ioopm_linked_list_append(all_values, entry->value);
        entry = entry->next;
      }
    }
  }

  return all_values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, table_key_equality, (void *)&key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, table_val_equality, (void *)&value);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < NO_BUCKETS; i++)
  {
    ioopm_entry_t *current = ht->buckets[i].next;

    while (current)
    {
      if (pred(ht->key_cmp_func, ht->val_cmp_func, current->key, current->value, arg))
      {
        return true;
      }
      current = current->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < NO_BUCKETS; i++)
  {
    ioopm_entry_t *current = ht->buckets[i].next;
    while (current)
    {
      if (pred(ht->key_cmp_func, ht->val_cmp_func, current->key, current->value, arg) == false)
      {
        return false;
      }
      current = current->next;
    }
  }
  return true;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_func, void *arg)
{
  for (int i = 0; i < NO_BUCKETS; ++i)
  {
    ioopm_entry_t *current = ht->buckets[i].next;

    while (current)
    {
      apply_func(current->key, &current->value, arg);
      current = current->next;
    }
  }
}