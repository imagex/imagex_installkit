## API and Hooks

#### Available Hooks

* [hook_imagex_installkit_install_tasks()]()
* [hook_imagex_installkit_install_tasks_alter()]()
* [hook_imagex_installkit_install_profile_modules_alter()]()
* [hook_imagex_installkit_install_block_info()]()
* [hook_imagex_installkit_default_theme()](#hook_imagex_installkit_default_theme())

#### hook_imagex_installkit_default_theme()

```php
/**
 * Hook to specify the default theme that should be enabled and set.
 *
 * The most derived installation profile's implementation of this hook will be
 * used for determining the default theme at installation. Therefore, as a
 * result, the imagex_installkit has implemented a top level version specifying
 * a theme to enable during installation if no child profile specifies one.
 * 
 * @return string
 *   The name of the theme to enable for default usage.
 */
function hook_imagex_installkit_default_theme() {
  return 'parrot';
}
```
