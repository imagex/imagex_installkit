<?php

/**
 * Wrapper class around the SPL RecursiveIteratorIterator.
 */
class EntityDependencyIteratorIterator extends RecursiveIteratorIterator {}

/**
 * Iterator class which does the heavy lifting for detecting dependencies.
 *
 * This iterator is reponsible for taking in an array of entity types and ids,
 * figuring out all their dependencies, and returning an iterable object of all
 * of them in a sane order (dependencies first). And since dependencies
 * in theory are nested and recursive, we are using a recursive iterator here.
 *
 * @todo
 *   We need to throw an exception when we detect a circular dependency.
 */
class EntityDependencyIterator implements RecursiveIterator {

  /**
   * The entities to be iterated over.
   *
   * @var array
   */
  public $entities = array();

  /**
   * The entity type of the entity currently being iterated over.
   *
   * @var string
   */
  public $entityType = NULL;

  /**
   * The entity ID of the entity currently being iterated over.
   *
   * @var string
   */
  public $entityId = NULL;

  /**
   * An array of dependencies to the entity being parsed.
   *
   * @var array
   */
  public $dependencies = array();

  /**
   * An array of belongings to the entity being parsed.
   *
   * @var array
   */
  public $belongings = array();

  /**
   * An array with information on who added a particular entity to the tree,
   * i.e. an entity's origin.
   *
   * @var array
   */
  public $origins = array();

  /**
   * Keeps track of entities that have already been checked for dependencies.
   *
   * @var array
   */
  public $checked = array();

  /**
   * Keeps track of entities that have already been traversed (output).
   *
   * @var array
   */
  public $traversed = array();

  /**
   * Constructor.
   *
   * @param array $entities
   *   A structured array of entity ids and their entity types.
   * @param array $parent
   *   The parent array of the current entity.
   *
   * @see entity_dependency_iterator().
   */
  public function __construct($entities, &$parent = NULL) {
    $this->entities = $entities;

    if (empty($parent)) {
      foreach ($this->entities as $entity_type => $entity_ids) {
        foreach ($entity_ids as $entity_id => $somethings) {
          $this->origins[$entity_type][$entity_id] = FALSE;
        }
      }
    }
    else {
      $this->origins =& $parent->origins;
      $this->checked =& $parent->checked;
      $this->traversed =& $parent->traversed;
    }
  }

  /**
   * Returns TRUE if an iterator can be created for the current item in the
   * entities array.
   *
   * @return boolean
   */
  public function hasChildren() {
    $current = current($this->entities);
    if (is_array($current)) {
      return TRUE;
    }

    // Don't check for dependencies twice.
    if (!isset($this->checked[$this->entityType][$this->entityId]) && !empty($this->entityId)) {
      // Load the current entity.
      $entity = reset(entity_load($this->entityType, array($this->entityId)));

      $this->dependencies = module_invoke_all('entity_dependencies', $entity, $this->entityType);
      //$this->belongings = module_invoke_all('entity_belongings', $entity, $this->entityType);

      // Don't add dependencies that already were checked.
      foreach ($this->dependencies as $entity_type => $dependencies) {
        foreach ($dependencies as $entity_id) {
          if (isset($this->checked[$entity_type][$entity_id])) {
            unset($this->dependencies[$entity_type][$entity_id]);
          }
          else {
            $this->origins[$entity_type][$entity_id] = array(
              'type' => $this->entityType,
              'id' => $this->entityId,
            );
          }
        }
      }
      // TODO: Do the same for belongings.

      // Let other modules have their say.
      drupal_alter('entity_dependencies', $this->dependencies, $entity, $this->entityType);
      //drupal_alter('entity_belongings', $this->belongings, $entity, $this->entityType);

      // Now mark this as checked.
      $this->checked[$this->entityType][$this->entityId] = TRUE;

      if (!empty($this->dependencies)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Helper method to get entity dependencies.
   */
  public function getChildrenEntities() {
    $entities = array();
    $current = current($this->entities);
    if (is_array($current)) {
      $entities = $current;
    }
    if (!empty($this->dependencies) || !empty($this->belongings)) {
      $entities = $this->dependencies;
      // In an iterator, having children means that the current key itself
      // isn't a part of the entities. However, we need that entity.. So we add
      // the parent as a part of the entities. And since children always should
      // go first, we add the parent last.
      $entities[$this->entityType][] = $this->entityId;

      // TODO: Add belongings to the array.
      //foreach ($this->belongings as $entity_type => $entity_id) {
      //  $entities[$entity_type][] = $entity_id;
      //}
    }
    return $entities;
  }

  /**
   * Returns an iterator for the current entry.
   *
   * @return EntityDependencyIterator
   */
  public function getChildren() {
    $iterator = new EntityDependencyIterator($this->getChildrenEntities(), $this);
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  /**
   * Get the current entity formatted with some extra metadata according to
   * the OData protocol.
   *
   * @see http://www.odata.org/developers/protocols
   */
  public function current() {
    $current = current($this->entities);
    if (is_array($current)) {
      return $current;
    }
    // Load the current entity.
    $entity = reset(entity_load($this->entityType, array($this->entityId)));
    // Add necessary metadata to the entity.
    $entity->__metadata = array(
      'type' => $this->entityType,
      'uri' => $this->entityType . '/' . $this->entityId,
    );
    // Now mark this as traversed.
    $this->traversed[$this->entityType][$this->entityId] = TRUE;
    return $entity;
  }

  /**
   * Returns the key of the current element.
   */
  public function key() {
    return key($this->entities);
  }

  /**
   * Moves the current position to the next element.
   */
  public function next() {
    return next($this->entities);
  }

  /**
   * Rewinds the Iterator to the first element.
   */
  public function rewind() {
    reset($this->entities);
  }

  /**
   * Checks if current position is valid.
   *
   * @return boolean
   */
  public function valid() {
    $current = current($this->entities);
    if (!empty($current) && is_array($current)) {
      $this->entityType = key($this->entities);
      return TRUE;
    }
    elseif (!empty($current) && is_numeric($current) && isset($this->entityType) && !isset($this->traversed[$this->entityType][$current])) {
      $this->entityId = $current;
      return TRUE;
    }
    return FALSE;
  }
}
