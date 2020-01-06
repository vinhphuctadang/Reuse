
class trie:
    def __init__ (self):
        self.__nxtchar = {}

    def insert (self, word):
        if (word == ""):
            return None
        if not word[0] in self.__nxtchar:
            __nxtchar[word[0]] = trie()        
        self.__nxtchar[word[0]].insert (word[1:])
        pass
    
    def hint (self, prefix): # return a word

        if prefix == "":
            tmp = ""
            node = self
            while len (node.__nxtchar) > 0:
                c = self.__nxtchar.keys[0]
                node = self.__nxtchar[c]
                tmp += c
            return tmp
        
        if prefix[0] not in self.__nxtchar:
            return ""

        return ''.join ([prefix[0], self.__nxtchar[prefix[0]].hint (prefix[1:])])
        pass
    
    
def main ():
    mytrie = trie ()
    mytrie.insert ('hello')
    mytrie.insert ('helli')
    mytrie.insert ('hex')

    print (mytrie.hint ('he'))

if __name__=='__main__':
    main ()
