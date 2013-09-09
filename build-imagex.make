api = 2
core = 7.x

; Include the definition of building core directly.
includes[] = "drupal-org-core.make"

; Download the ImageX install profile base and its dependencies from GitHub.
projects[imagex_installkit][type] = "profile"
projects[imagex_installkit][download][type] = "git"
projects[imagex_installkit][download][url] = "git@github.com:imagex/imagex_installkit.git"
projects[imagex_installkit][download][branch] = "7.x-dev"

; Download the ImageX modules
projects[imagex][type] = "module"
projects[imagex][subdir] = "imagex"
projects[imagex][download][type] = "git"
projects[imagex][download][url] = "git@github.com:imagex/imagex.git"
projects[imagex][download][branch] = "7.x-dev"