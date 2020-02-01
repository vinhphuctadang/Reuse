class trie:
    limitation = 64
    def __init__ (self):
        self.__nxtchar = {}

    def insert (self, word):
        if len (word) > trie.limitation:
            raise ValueError ('word could not be greater than {}'.format (trie.limitation))
        if (word == ""):
            return None
        if not word[0] in self.__nxtchar:
            self.__nxtchar[word[0]] = trie()        
        self.__nxtchar[word[0]].insert (word[1:])
        pass
    
    def hint (self, prefix): # return a word

        # print (prefix)
        if prefix == "":
            tmp = ""
            node = self
            while len (node.__nxtchar) > 0:
                # print (len (node.__nxtchar))
                c = next (iter(node.__nxtchar))
                node = node.__nxtchar[c]
                tmp += c
            return tmp
        
        if prefix[0] not in self.__nxtchar:
            return None


        tmp = self.__nxtchar[prefix[0]].hint (prefix[1:])
        if tmp == None:
            return None
        return ''.join ([prefix[0], tmp])
    
    
def main ():
    mytrie = trie ()
    
    lst = ['aaaa', 'aa']

    for wrd in lst: 
        mytrie.insert (wrd)
    # mytrie.insert ('hex')

    print (mytrie.hint ('a'))

if __name__=='__main__':
    main ()
