import conan
import typing

from conan import ConanFile


class GraphSerializer(ConanFile):
    generators: typing.List[str] = ['CMakeToolchain', 'CMakeDeps']
    settings: typing.List[str] = ['os', 'compiler', 'build_type', 'arch']

    def __init__(self: 'GraphSerializer', display_name: str = '') -> None:
        self._requirements = {
            'gtest': '1.14.0'
        }
        super().__init__(display_name)

    def requirements(self: 'GraphSerializer') -> None:
        for req in self._requirements:
            self.requires(f'{req}/{self._requirements[req]}')

    def layout(self: 'GraphSerializer') -> None:
        conan.tools.cmake.cmake_layout(self)
