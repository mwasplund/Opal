Name: "Opal.UnitTests"
Language: "C++|0.4"
Version: "1.0.0"
Type: "Executable"
Source: [
	"gen/Main.cpp"
]
IncludePaths: [
	"./"
]

Dependencies: {
	Runtime: [
		"Soup.Test.Assert@0.2.0"
		"../Source/"
	]
}